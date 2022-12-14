#include "std_lib.h"   // needed because printf may be rewritten to call helpers defined here

struct D { int d1; struct D *d2; };
struct C { struct D c1; long c2, c3, c4, c5, c6; };
struct A { struct A *a1; struct C *a2; };
struct B { struct C b1; struct A *b2; };


void foo (struct B *x, struct B *y)
{
  struct B bb;
  bb.b1.c1.d1 = 333;
  struct C cc;
  cc.c2  = 444;
  struct A aa;
  aa.a1 = &aa;
  aa.a2 = &cc;
  struct A* pa = &aa;


  if (pa->a2->c2 != 444) {
    abort ();
  }

  
  if (x->b2 == 0)
    {
      struct A *a;

      x->b2 = a = y->b2;
      y->b2 = 0;
      for (; a; a = a->a1)
	a->a2 = &x->b1;
    }

  if (y->b2 != 0)
    abort ();

  if (x->b1.c3 == -1)
    {
      x->b1.c3 = y->b1.c3;
      x->b1.c4 = y->b1.c4;
      y->b1.c3 = -1;
      y->b1.c4 = 0;
    }

  if (y->b1.c3 != -1)
    abort ();
}

struct B x, y;

int main ()
{
  y.b1.c1.d1 = 6;
  y.b1.c3 = 145;
  y.b1.c4 = 2448;
  x.b1.c3 = -1;
  foo (&x, &y);
  return 0;
}
