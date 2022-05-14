#include <cmath>
#include <string>

/** Return extractd integer starting at str[i]. Update i to new end index */
int extract_int(const char* str, int len, int& i) {
    int nlen = 0, n = 0, j = i;
    while (nlen + i < len && str[nlen + i] >= '0' && str[nlen + i] <= '9')
        nlen++;
    int p = nlen - 1;
    while (j < i + nlen) n += (str[j++] - 48) * pow(10, p--);
    i = j;
    return n;
}

/** Return extractd integer */
int extract_int(const char* str, int len) {
    int i;
    return extract_int(str, len, i);
}