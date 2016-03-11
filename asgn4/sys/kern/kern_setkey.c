/*-
 * Copyright (c) 2016,	Adam Henry <adlhenry@ucsc.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <sys/cdefs.h>

#include <sys/systm.h>
#include <sys/sysproto.h>
#include <sys/kernel.h>
#include <sys/syscallsubr.h>
#include <sys/setkey.h>
#include <sys/libkern.h>
#include <sys/param.h>
#include <sys/malloc.h>

static MALLOC_DEFINE(M_KEYSET, "keyset", "Key storing data structures");

keyset *keylist[KS_MAXKEYS];
u_int keylist_index = 0;

char *
sk_getkey(uid_t uid)
{
	struct keyset *key;
	char *hexkey;
	int i;
	
	hexkey = NULL;
	for (i = 0; i < keylist_index; i++) {
		key = keylist[i];
		if (key->ks_uid == uid) {
			hexkey = key->ks_hexkey;
		}
	}
	return hexkey;
}

static inline char *
sk_hexkey(u_int k0, u_int k1)
{
	if (k0 == 0 && k1 == 0) return NULL;
	size_t buflen = 17;
	char hexkey[buflen];
	snprintf(hexkey, buflen, "%08x%08x", k0, k1);
	return hexkey;
}

static inline int
sk_insertkey(uid_t uid, u_int k0, u_int k1)
{
	struct keyset *key;
	int error, i, nullkey;
	
	error = 0;
	nullkey = -1;
	
	key = NULL;
	for (i = 0; i < keylist_index; i++) {
		if (!keylist[i]->ks_hexkey) nullkey = i;
		if (keylist[i]->ks_uid == uid) {
			key = keylist[i];
			break;
		}
	}
	
	if (!key) {
		key = malloc(sizeof(struct keyset *), M_KEYSET, M_WAITOK);
	}
	key->ks_uid = uid;
	key->ks_k0 = k0;
	key->ks_k1 = k1;
	key->hexkey = sk_hexkey(k0, k1);
	
	if (i < keylist_index) {
		return (error);
	}
	if (nullkey != -1) {
		free(keylist[nullkey], M_KEYSET);
		keylist[nullkey] = key;
	} else if (keylist_index < KS_MAXKEYS) {
		keylist[keylist_index++] = key;
	} else {
		error = -1;
	}
	return (error);
}

#ifndef _SYS_SYSPROTO_H_
struct setkey_args {
	u_int	k0;
	u_int	k1;
};
#endif
int
sys_setkey(struct thread *td, struct setkey_args *uap)
{
	int error;
	uid_t uid;
	u_int k0, k1;
	
	error = 0;
	uid = td->td_proc->p_ucred->cr_uid;
	k0 = uap->k0;
	k1 = uap->k1;
	if (!sk_getkey(uid)) {
		if (k0 == 0 && k1 == 0) {
			return KS_INVALIDKEY;
		}
	}
	if (sk_insertkey(uid, k0, k1) == -1) {
		return KS_KEYLISTFULL;
	}
	return (error);
}
