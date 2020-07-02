#ifndef _CHAYKIN_OS_NMI_H_
#define _CHAYKIN_OS_NMI_H_ 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void nmi_enable(void);
void nmi_disable(void);

#ifdef __cplusplus
}
#endif

#endif
