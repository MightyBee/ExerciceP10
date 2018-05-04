#include "Dessinable.h"
using namespace std;

Dessinable::Dessinable(SupportADessin* sup) : support(sup) {}

void Dessinable::set_sup(SupportADessin* sup){
  support=sup;
}
