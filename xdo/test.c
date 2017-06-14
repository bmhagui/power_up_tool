#include <stdio.h>
#include "/home/bhagui/bin_stage/xdo/xdotool-master/xdo.h"

int main() {
  xdo_t *xdo = xdo_new(NULL);
  xdo_keysequence(xdo, CURRENTWINDOW, "A", 0);
  return  0;
}
