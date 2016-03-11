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
 *
 */

#ifndef _SYS_SETKEY_H_
#define	_SYS_SETKEY_H_

#define KS_MAXKEYS 		16	/* Number of keys storable. */
#define KS_INVALIDKEY 	1	/* Attempt to remove a non-existent key. */
#define KS_KEYLISTFULL 	2	/* The keylist has reached its capacity. */

#ifdef _KERNEL
struct keyset {
	uid_t	ks_uid;		/* User id of the key owner. */
	u_int	ks_k0;		/* First half of the user key. */
	u_int	ks_k1;		/* Second half of the user key. */
	char	*ks_hexkey; /* The 16-digit hex string version of the key. */
};

char *sk_getkey(uid_t uid);
#else
__BEGIN_DECLS
int	setkey(u_int, u_int);
__END_DECLS
#endif
#endif /* !_SYS_SETKEY_H_ */
