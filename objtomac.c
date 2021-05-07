/* OBJTOMAC.C
 *
 * Convert HI-TECH C OBJ file to MAC file, ready to run through
 * M80 to produce REL. It is slightly painful to assemble twice
 * (once with ZAS and then with M80), but this shields us from
 * the details of REL files. Indeed, other linkers can be
 * supported the same way (by modifying the MAC output and then
 * re-assembling.
 *
 * OBJTOMAC is an extension of AS2MAC, which converted (crudely)
 * the AS file into a MAC file. This was ok for converting the
 * HI-TECH C run-time library, but did not convert arbitrary ZAS
 * source, and had problems with converting some of the
 * productions from the C compiler. Note that producing
 * assembler with HI-TECH 3.09 does not always work properly!
 * Sometimes the AS file is not valid! So, we go the route of
 * converting the OBJ files instead.
 *
 * OBJ files support text, data and bss. Other names are
 * possible, but not used by the C compiler. REL supports CSEG
 * and DSEG. We map text to CSEG and data AND bss to DSEG. True
 * uninitialized data is possible in REL using /COMMON/. We
 * would need to also link with PLINK-II.
 */

#include <stdio.h>
#include <string.h>

/* OBJ packet buffer. This appears to always be 500 bytes or
 * so maximum. We make it 1024 for a margin of error.
 */
#define PSIZ 1024
int  len = 0;
char typ = 0;
char buf[PSIZ + 6] = { 0, };

/* Copy of the data payload for relocation. Relocation does
 * refer back to the bytes loaded. We are assuming that we
 * need keep only the most recent data payload.
 */
char buf2[PSIZ] = { 0, };

/* HI-TECH OBJ is composed of packets: len, type, data
 */
int read_packet(FILE *f) {
    int i;
    i = fread(&len, 1, 2, f);
    if (i != 2)
	return 0;
    if ((len < 0) || (len > sizeof(buf))) {
	printf(" ERROR\n");
	return 0;
    }
    i = fread(&typ, 1, 1, f);
    if (i != 1)
	return 0;
    if (len > 0) {
	i = fread(buf, 1, len, f);
	if (i != len)
	    return 0;
    }
    /* Put 6 byte sentinel at end. This makes it much easier to
     * find the payload end sometimes.
     */
    for (i = len; i < (len + 6); ++i)
	buf[i] = 0xff;
    return 1;
}

/* MAC file preamble
 */
void preamble(void) {
    printf("; GENERATED BY OBJTOMAC 0.50\n");
    printf(" CSEG\n");
    printf("text:\n");
    printf(" DSEG\n");
    printf("data:\n");
}

/* MAC file postamble
 */
void postamble(void) {
    printf(" END\n");
}

/* Enter data bytes - into text or data
 */
void data(void) {
    int i, j, o;
    char *s, *d;

    o = *(int *)buf;
    s = buf + 4;
    d = buf + 4 + strlen(s) + 1;
    i = len - strlen(s) - 1 - 4;
    if (strcmp("text", s) == 0) {
	printf(" CSEG\n");
	printf("OFFSET SET 0%04xH\n", o);
	printf(" ORG OFFSET\n");
    } else if (strcmp("data", s) == 0) {
	printf(" DSEG\n");
	printf("OFFSET SET 0%04xH\n", o);
	printf(" ORG OFFSET\n");
    } else {
	printf(" DSEG\n");
	printf("bss: ds 0%04xH\n", o);
    }
    for (j = 0; j < i; ++j) {
	if ((j & 7) == 0)
	    printf(" DB ");
	printf("0%02xH", d[j] & 0xff);
	buf2[j] = d[j]; /* copy for relocations */
	if (j == (i - 1)) {
	    printf(" ; %04x\n", j - (j & 7));
	    break;
	} else if ((j & 7) != 7)
	    printf(",");
	else {
	    printf(" ; %04x\n", j - 7);
	}
    }
}

/* Declare public and external symbols. This is not done when
 * the code or data is defined. We back up to the location
 * with ORG, define the symbol and make it public. A similar
 * approach is used in reloc. This does rely heavily on M80,
 * but allows us to do the conversion with ease.
 */
void public(void) {
    int i, o;
    char *s, *d, *p;

    p = buf;
    for (;;) {
	i = *(int *)(p + 4);
	if (i == 0xffff) /* use sentinel for end */
	    break;
	o = *(int *)p;
	s = p + 6;
	d = p + 6 + strlen(s) + 1;
	p = d + strlen(d) + 1;
	if (i != 0) {
	    if (*s == '\0')
		printf(" EXTRN %s\n", d);
	    else {
		if (strcmp(s, "text") == 0)
		    printf(" CSEG\n");
		else
		    printf(" DSEG\n");
		if (strcmp(s, "bss") == 0)
		    printf(" ORG bss+0%04xH\n", o);
		else
		    printf(" ORG 0%04xH\n", o);
		printf("%s:\n", d);
		printf(" PUBLIC %s\n", d);
	    }
	}
    }
}

/* Process relocations. Back up over code, and put in
 * relocation.
 */
void reloc(void) {
    int i, j, t;
    char *s, *p;

    p = buf;
    for (;;) {
	i = *(int *)p;
	if (i == 0xffff) /* use sentinel for end */
	    break;
	t = p[2];
	s = p + 3;
	j = *(int *)(buf2 + i);
	printf(" ORG OFFSET+0%04xH\n", i);
	if (j == 0)
	    printf(" DW %s\n", s);
	else
	    printf(" DW %s+0%04xH\n", s, j);
	p = p + 3 + strlen(s) + 1;
    }
}

int main(int ac, char **av) {
    FILE *f;

    /* open input file */
    f = fopen(av[1], "rb");
    if (f == NULL) {
	printf("can't open %s\n", av[1]);
	return 1;
    }

    /* read and process packets */
    while (read_packet(f)) {
	switch (typ) {
	case 7:
	    preamble();
	    break;
	case 6:
	    postamble();
	    fclose(f);
	    return 0;
	case 1:
	    data();
	    break;
	case 2:
	    break;
	case 3:
	    reloc();
	    break;
	case 4:
	    public();
	    break;
	/* On error (unknown packet type), put out an error
	 * into the MAC output
	 */
	default:
	    printf(" ERROR\n");
	}
    }
    return 0;
}
                                