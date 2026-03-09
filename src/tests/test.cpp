#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <source_location>
#include <sstream>

template <typename T>
constexpr std::string try_tostr(const T& t){
  if constexpr(requires{ (std::ostringstream() << t).str(); }){
    return (std::ostringstream() << std::boolalpha << t).str();
  } else {
    return "<obj>";
  }
}

struct info_t {
  std::string_view expr = "";
  std::source_location sl = std::source_location::current();
  bool result = false;
  std::string opt_lhs = "";
  std::string opt_rhs = "";
};

template <class A>
struct check_t {
 A a;
  info_t& i;
#define CHECK_T_COMP_OP(OP) \
  template <class B> \
  info_t operator OP(B&& b) && { \
    i.opt_lhs = try_tostr(a); \
    i.opt_rhs = try_tostr(b); \
    i.result = (a OP b);\
    return std::move(i);\
  } static_assert(true)
  CHECK_T_COMP_OP(==);
  CHECK_T_COMP_OP(!=);
  CHECK_T_COMP_OP(> );
  CHECK_T_COMP_OP(>=);
  CHECK_T_COMP_OP(< );
  CHECK_T_COMP_OP(<=);
#undef CHECK_T_COMP_OP
  operator info_t() && {
    i.opt_lhs = try_tostr(a);
    i.result = a;
    return std::move(i);
  }
};

struct destruct_t {
  info_t i;
  template <class A>
  check_t<A> operator<(A&& a) && {
    return check_t<A>{std::forward<A>(a), i};
  }
};
#define DECOMPOSE_CMP_OP(expr, lit) destruct_t{info_t{lit ""}} < expr

void check_internal(info_t i, std::string_view additional_info = ""){
  if(not i.result){
    std::cerr << "\033[31m" << i.sl.file_name() << ":" << i.sl.line() << ":" " epxr: " << i.expr << " , with values:  " << i.opt_lhs << additional_info << "\n";
  }
  else{
	std::cout << "\033[32m" << i.sl.file_name() << ":" << i.sl.line() << " | " << "Success!\n";
  }
}
#define CHECK(e, ...) check_internal(DECOMPOSE_CMP_OP(e,#e) __VA_OPT__(,) __VA_ARGS__)

struct testfunction{
  using function_sig = void();
  function_sig& function;
  std::string_view name;
};

#ifdef DISABLE_TESTS
#define TEST_CASE(name) static void name()
constinit struct {
  void execute_tests(){
	std::cout << "Reached this part.";
  }
} runner;
#else
constinit struct {
  std::vector<testfunction> tests = {};
  int add(testfunction test) {
    tests.push_back(test);
    return 0;
  }
  void execute_tests(){
    
    for(const auto& t : tests){
      try{
	std::cout << "\033[37m" << t.name << "\n";
        t.function();
      } catch(...){
        std::cerr << "test " << t.name << " failed\n";
      }
    }
  }
} runner;

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define TEST_CASE(name) static void name(); int CONCAT(impl, __LINE__) = runner.add({name, #name}); static void name()
#endif

int add(int a, int b) {return a + b;}

int subtract(int a, int b) {return a-b;}

TEST_CASE(test1) {
  CHECK(add(1, 2) == 3);
  CHECK(add(4, 5) == 9);
};

TEST_CASE(test2) {
  CHECK(add(1, 2) == 3);
  CHECK(false);
  CHECK(not true);
};

TEST_CASE(test3) {
  CHECK(subtract(2,1) == 0);
};

int main() {
  
  runner.execute_tests();
}
