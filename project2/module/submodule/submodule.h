#ifndef __SUBMODULE_H
#define __SUBMODULE_H

/* Use ioremap to control devices */

/* physical address */
#define FND_ADDRESS 0x08000004
#define LED_ADDRESS 0x08000016
#define DOT_ADDRESS 0x08000210
#define TEXT_LCD_ADDRESS 0x08000090

/* text lcd string */
#define STUDENT_NUMBER "20161562"
#define STUDENT_NAME "Junghee Kye"

/* ioremap devices */
int submodule_ioremap(void);

/* iounmap devices */
int submodule_iounmap(void);

/* initialize devices */
int submodule_init(char*);

/* update devices */
int submodule_update(void);

/* clear devices */
int submodule_clear(void);

#endif
