#ifndef INPUT_H
#define INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

extern void input_init(char *ptr);
extern char input_charAt(int pos);
extern char input_next();
extern char input_peek(void);
extern int input_eof(void);
extern void input_croak(char *str);

#ifdef __cplusplus
}
#endif

#endif /* INPUT_H */
