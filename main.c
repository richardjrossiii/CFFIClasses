#include "Foo.h"

int main() {
    FooType foo = FooClass.create();

    foo->foo();
    printf("result of bar is: %i\n", foo->bar(10));

    foo->destroy();
}
