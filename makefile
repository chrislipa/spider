#You'll have to set the -L and -I flags to the right library path
#
CFLAGS =  -I/usr/openwin/include -g -L/usr/local/openwin/lib
OBJS = ray_routines.o spider.o julia_routines.o modal_quad_routines.o \
				k_iterate.o  k_driver.o k_main.o
LIBS = -lm -lxview -lolgx -lX11


spider : $(OBJS)
	cc  $(CFLAGS)  -o spider $(OBJS) $(LIBS)

modal_quad_routines.o: modal_quad_routines.c spider.h
	cc $(CFLAGS) -c modal_quad_routines.c $(LIBS)

ray_routines.o: ray_routines.c spider.h cmath.h
	cc $(CFLAGS) -c ray_routines.c $(LIBS)

julia_routines.o: julia_routines.c spider.h cmath.h
	cc $(CFLAGS) -c julia_routines.c $(LIBS)

spider.o: spider.c spider.h cmath.h
	cc $(CFLAGS) -c spider.c $(LIBS)


k_driver.o : k_driver.c koebe.h
	cc $(CFLAGS) -c -o k_driver.o k_driver.c

k_iterate.o : k_iterate.c koebe.h
	cc $(CFLAGS) -c -o k_iterate.o k_iterate.c

k_main.o : k_main.c koebe.h
	cc $(CFLAGS) -c -o k_main.o k_main.c
