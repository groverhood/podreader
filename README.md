# podreader

The goal of this header-only library is to provide a file-based analogue to POD structures in the form of
JavaScript Object Notation. This is achieved through compile-time reflection utilities, and runtime tools
that make use of said utilities to handle types dynamically.

## Example

Using a file called 'test.json', which appears as such:

```json
{
  "first": 1,
  "second": 2,
  
  "third": {
    "nested": 3.14,
    "other": 12
  },
  
  "fourth": 1.41
}
```

And a cxx file that is written as such:

```cpp

#include <json\jsonreader.hxx>
#include <json\operator.hxx>

#include <ifstream>
#include <iostream>

struct my_test
{
  int first, second;
  struct inner
  {
    double nested;
    short other;
  } third;
  
  float fourth;
};

int main()
{
  struct my_test test;
  std::ifstream is("test.json");
  
  is >> test;
  
  std::cout << "{ " << test.first << ", " << test.second << " { " << test.third.nested
            << ", " << test.third.other << "}, " << test.fourth << " }" << std::endl;
  
  return 0;
}

```

We will get the output "{ 1, 2, { 3.14, 12 }, 1.41 }"

##Planned Features

Currently there is no support for strings, due to there being no safe string POD type that also manages 
its own pointer, which is a must for file I/O. Outputting POD structs into .json files/strings is also 
a feature I plan on adding soon.
