#include <stdio.h>


double power(int num, int pow)
{
  if (pow < 0)
    return 1 / power(num, -pow);
  
  if (pow == 0)
    return 1;

  if (pow == 1)
    return num;

  return power(num, pow - 1);

}

int main()
{
  printf("power 3^4 is %f\n", power(3, 4));
}


















