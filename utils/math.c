#include "typedefs.h"
#include "../drivers/vga_text.h"
#include "math.h"


/* integer powers of a float */
float pow_f (float x, uint32_t n) {
    float res = 1.0;

    for (uint32_t i = 0; i < n; i++)
        res *= x;

    return res;
}

/* auxiliary function for ln */
float aux_ln (int x) {
    return ((float)x - 1) / ((float)x+1);
}


/* returns approx of ln */
/* does not quite scale, maybe implement this with newton*/
float ln (float x, uint32_t order) {
    float res = 0;
    for (uint32_t i = 1; i < order; i+=2){
        res += (1.0 / (float) i) * pow_f(aux_ln(x), i);
    }
    return 2 * (res);
}

/* lookup values for log */
float ln2  = 0.6931471805599453;
float ln10 = 2.302585092994046;
float ln16 = 2.772588722239781;

/* returns ceil of log approx */
uint32_t ceil_log (uint32_t x, uint32_t base) {
    if (base == 2)
        return CEILING(ln(x, 1000) / ln2);

    if (base == 10)
        return CEILING(ln(x, 1000) / ln10);

    if (base == 16)
        return CEILING(ln(x, 1000) / ln16);

    kprint("Error: non-implemented base for ceil log");

    for(;;);
}
