#include <iostream>
#include <string>
using namespace std;
class bar
{
public:
    bar() :m_ptr(new int(0))
    {

    }

    bar(const bar& a) :m_ptr(new int(*a.m_ptr)) //深拷贝
    {
        std::cout << "copy construct" << std::endl;
    }

    ~bar()
    {
        delete m_ptr;
    }

private:
    int* m_ptr;
};

bar Get(bool flag)
{
    bar a;
    bar b;
    if (flag)
        return a;
    else
        return b;
}

bar GetA()
{
    bar a;
    return a;
}
int right_value_ref2()
{ 
    bar aa= GetA();
    //bar a = Get(false);  //运行报错

    return 0;
}
class foo
{
public:
    foo():a(2),b(new int(3)) {
        cout << "foo"<<endl;
    };
    void print()
    {
        cout << "a:" << a << endl;
        if (b==nullptr)
        {
            cout << "b:nullptr" << endl;
        }
        else
            cout << "b:" << *b << endl;
    }
    foo(foo&& other) {
        cout << "foo&& ";
        b = new int(*other.b);
        delete other.b;
        other.b = nullptr;
    };
private:
    int a = 0;
    int* b =nullptr;
};
void printInt(int & i)
{
    cout << "左值引用：" << i << endl;
}

void  printInt(int && i)
{
    cout << "右值引用：" << i << endl;
}
void main()
{
    right_value_ref2();
    cout << "hello world" << endl;
    int i = 2;
    printInt(i);
    printInt(3);
    printInt(move(i));
    printInt(i);

    foo f1;
    f1.print();
    auto bar1 = move(f1);
    cout << "bar1" << endl;
    bar1.print();
    cout << "bar" << endl;
    f1.print();
    //system("pause");
}