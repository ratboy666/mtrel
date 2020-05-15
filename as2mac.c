/*
 * as2mac.c
 *
 * Convert as to mac - specific to C production
 * This is used to allow combining Hi-Tech C with
 * Microsoft and Digital Research products.
 *
 * This is a quick hack -- needs to convert output
 * from cgen and optim -- nothing else. We can then
 * rebuild the run-time libraries, manually converting
 * the assembler as needed. no error checking, etc.
 * the as output is assumed correct, and the format
 * is assumed not to change. Since the source for cgen
 * may be (or likely) lost, this is safe. If using
 * m80/l80 then assembler will be written in mac
 * directly, so as2mac is ONLY needed to convert the
 * output of C (or OPTIM).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys.h>

int changes;
char pbuf[128 +32];
char buf[128 + 32];
char buf2[128 + 32];
struct label {
    struct label *p;
    int line;
    char s[14];
};

/* label ddd: will be replace with Xline: where line is line number
 * (in AS file). original label retained in s.
 *
 * think -- list is in reverse order:
 *
 * 1:
 *    jp 1f
 * 1:
 *
 *
 * jp refers to second 1: if we search from end (reverse),
 * first 1: line >, second < -- keep line# if >
 */

struct label *labels = NULL;

int fline(char *s, int l) {
    struct label *lp;
    int line = 0;
    for (lp = labels; lp; lp = lp->p) {
        if (strcmp(s, lp->s) == 0) {
        if (lp->line > l)
            line = lp->line;
        }
    }
    return line;
}

FILE *f;

void pass1(void) {
    int line, i;
    char c;
    struct label *lp;

    /* pass 1 - get all labels _label: digit...:  */
    line = 0;
    while (fgets(buf, sizeof (buf), f)) {
        ++line;
        c = buf[0];
        if ((c == '_') ||
            ((c >= '0') && (c <= '9'))) {
            lp = malloc(sizeof (*lp));
            for (i = 0; i < 14; ++i) {
                if (buf[i] == ':')
                    break;
                lp->s[i] = buf[i];
            }
            lp->s[i] = '\0';
            lp->line = line;
            lp->p = labels;
            labels = lp;
        }
    }
}

int stc(char c) {
    return (c == ' ') || (c == '\t') || (c == ',');
}

void l2q(void) {
    /* m80 not case sensitive - L -> Q */
    char *s;
    if (buf[0] == 'L')
        buf[0] = 'Q';
    for (s = buf; *s; ++s)
        if (stc(*s))
            if (s[1] == 'L') {
                ++changes;
                s[1] = 'Q';
	    };
}

void hilo(void) {
    char *s;
    /* .high. and .low. => high and low  */
    for (s = buf; *s; ++s)
        if (*s == '.') {
            if (strncmp(".high.", s, 6) == 0) {
		++changes;
                *s = ' ';
                s[5] = ' ';
            } else if (strncmp(".low.", s, 5) == 0) {
		++changes;
                *s = ' ';
                s[4] = ' '; 
            }
        }
}

void deff(void) {
    char *s;
    unsigned char *u;
    double d;
    /* deff => floating point conversion */
    for (s = buf; *s; ++s) {
        if (strncmp("deff", s, 4) == 0) {
            /* convert s + 5..n => float, dump as 4xbyte */
	    ++changes;
            d = atof(s + 5);
            u = (unsigned char *)&d;
            sprintf(s, "defb %d,%d,%d,%d\n", u[0], u[1], u[2], u[3]);
            return;
        } 
    }
}

void sects(void) {
    /* psect to cseg/dseg */
    if (strncmp("psect\ttext", buf, 10) == 0) {
	++changes;
        strcpy(buf, "cseg\n");
    } else if (strncmp("psect\t", buf, 6) == 0) {
	++changes;
	strcpy(buf, "dseg\n");
    }
}

void lab(int line) {
    /* fix forward labels: ddd:\n */
    char *s;
    if ((buf[0] < '0') || (buf[0] >= '9'))
        return;
    ++changes;
    s = buf;
    while (*s != ':')
        ++s;
    strcpy(buf2, s + 1);
    sprintf(buf, "X%d:%s", line, buf2); 
}

void glob(void) {
    /* global to public/extrn */
    int i;
    char *s;
    struct label *lp;
    if (strncmp("global", buf, 6) != 0)
        return;
    ++changes;
    i = 0;                                                             
    s = buf + 7;                                                       
    while (*s != '\n')                                                 
        ++s;                                                           
    *s = '\0';
    s = buf + 7;
    for (lp = labels; lp != NULL; lp = lp->p)
        if (strcmp(lp->s, s) == 0) {
            i = 1;
            break;
        }
    if (i)
        strcpy(buf2, "public ");
    else 
        strcpy(buf2, "extrn ");
    strcat(buf2, buf + 6);
    strcpy(buf, buf2);
    strcat(buf, "\n");
}

void fixand(void) {
    char *s;
    char *t = buf2;
    for (s = buf; *s; ++s) {
        if (*s != '&')
            *t++ = *s;
        else {
	    ++changes;
            *t++ = ' ';
            *t++ = 'A';
            *t++ = 'N';
            *t++ = 'D';
            *t++ = ' ';
        }
    }
    *t = '\0';
    strcpy(buf, buf2);
}

void fixjp(void) {
    /* jp ?,l conversion alt=m llt=c age=p lge=nc */
    /* anz az lnz lz fge flt fnz (undoc) */
    if (strncmp("jp\talt,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
        buf[4] = ' ';
        buf[5] = 'm';
    } else if (strncmp("jp\tllt,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
        buf[4] = ' ';
        buf[5] = 'c';
    } else if (strncmp("jp\tage,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
        buf[4] = ' ';
        buf[5] = 'p';
    } else if (strncmp("jp\tlge,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
        buf[4] = 'n';
        buf[5] = 'c';
    } else if (strncmp("jp\taz,", buf, 6) == 0) {
	++changes;
        buf[3] = ' ';
    } else if (strncmp("jp\tlz,", buf, 6) == 0) {
	++changes;
        buf[3] = ' ';
    } else if (strncmp("jp\tanz,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
    } else if (strncmp("jp\tlnz,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
    } else if (strncmp("jp\tfge,", buf, 7) == 0) {
        ++changes;
        buf[3] = ' ';
        buf[4] = ' ';
        buf[5] = 'p';
    } else if (strncmp("jp\tflt,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
        buf[4] = ' ';
        buf[5] = 'm';
    } else if (strncmp("jp\tfnz,", buf, 7) == 0) {
	++changes;
        buf[3] = ' ';
    }
}

void refs(int line) {
    /* fix forward references */
    char *s;
    char *t;
    char c;
    int state, i;
    s = buf;
    state = 0;
    while (*s) {
        if (state == 00) {
            if (stc(*s))
                state = 1;
            else
                state = 0;
        } else if (state == 1) {
            if (stc(*s))
                state = 1;
            else if ((*s >= '0') && (*s <= '9')) {
                t = s;
                state = 2;
            } else
                state = 0;
        } else if (state == 2) {
            if ((*s >= '0') && (*s <= '9'))
                state = 2;
            else if (*s == 'f')
                break;
            else
	        state = 0;
        }
        ++s;
    }
    if ((state == 2) && (*s == 'f')) {
        /* buf to t-1 t..s s..0 */
        ++changes;
        /* part 1 if buf up to t */
        c = *t;
        *t = '\0';
        strcpy(buf2, buf);
        *t = c;
/*      printf("******* part 1 = %s\n", buf2); */
	/* part 2 is t.. s */
        *s = '\0';
        i = fline(t, line);
/*	printf("******* part 2 = %s\n", t); */
/*      printf("******* part 3 = %s\n", s + 1); */
        *s = 'f';
	sprintf(buf2 + strlen(buf2), "X%d%s", i, s + 1);
        strcpy(buf, buf2);
    }
}

void pass2(char *name) {
    int line;
    line = 0;
    /* pass 2 - read lines, output mac global extrn public */
    printf("; as2mac %s\n", name);
    puts(".z80");
    while (fgets(buf, sizeof (buf), f)) {

	changes = 0;
	strcpy(pbuf, buf);
        ++line;
	if (buf[0] != ';') {
            l2q();
            hilo();
            deff();
	    sects();
            lab(line);
            glob();
            fixand();
            fixjp();
	    refs(line);
        }
	if (changes)
	    printf("; == %s", pbuf);
        fputs(buf, stdout);
    }
    /* maco-80 really wants an end */
    puts("end");
}

int main(int ac, char **av) {
    av = _getargs((char *)0x81, "as2mac");
    ac = _argc_;
    f = fopen(av[1], "r");
    if (f == NULL) {        
        printf("cannot open file %s\n", av[1]);
        return 1;
    }
    pass1();
    rewind(f);
    pass2(av[1]);
    fclose(f);
    return 0;
}
