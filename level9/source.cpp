#include <cstring>

class N
{
public:
    N(int value) : data(value) {}
    void setAnnotation(char *s)
    {
        memcpy(annotation, s, strlen(s));
    }
    int operator+(N &other)
    {
        return data + other.data;
    }
    int operator-(N &other)
    {
        return data - other.data;
    }

private:
    int data;
    char annotation[100];
};

int main(int argc, char **argv)
{
    if (argc <= 1)
        return 1;

    N n1(5); // 0x0804a008
    N n2(6); // 0x0804a078

    n1.setAnnotation(argv[1]); // 0x804a00c

    int result = n2 + n1;
    return result;
}
