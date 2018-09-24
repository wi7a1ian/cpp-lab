#pragma once

/*

------------------------------------------------------------------------------------------
My old code:
Inline (one line) functions in separate header file.
std::function used as a pointer to a function:			typedef std::function<double(double x)> ActivationFunctionPtr;
supported random and static cast:						std::mt19937 random_generator(static_cast<int>(time(0))); std::uniform_real<double> random01(0, 1);
final:													class ZeroWeightInitializer final; ... bool ComputeOutput(InputLayer const& inputLayer) override final;
override:												void InitializeWeights(WeightedNetwork& network) override;
delete:													KohonenNetwork() = delete; // The default constructor is explicitly stated.
throw:													bool ComputeOutput(InputLayer const& inputLayer) throw()
explicit:												explicit MultilayerPerceptron(std::initializer_list<int> networkLayerMap);
decltype:
foreach:												for each (auto layerSize in layerSizes)		for (const auto& layerSize : layerSizes)
const function:											SimulatedAnnealingConfig const& GetElmAnnealingConfig() const;
assert

------------------------------------------------------------------------------------------
https://github.com/lefticus/cppbestpractices/blob/master/00-Table_of_Contents.md

Do not pass and return simple types by const ref
Avoid Raw Memory Access - std::make_unique<char[]>, std::unique_ptr<MyClass>
Use std::array or std::vector Instead of C-style Arrays
Use Exceptions
Use C++-style cast instead of C-style cast
Avoid Compiler Macros
Consider Avoiding Boolean Parameters
Avoid Raw Loops
Properly Utilize 'override' and 'final'
Use Initializer Lists
Never Use std::bind -  Instead simply use a lambda.

------------------------------------------------------------------------------------------
https://google.github.io/styleguide/cppguide.html
Define functions inline only when they are small, say, 10 lines or fewer.
Use the explicit keyword for conversion operators and single-argument constructors.
Avoid virtual method calls in constructors, and avoid initialization that can fail if you can't signal an error.
Support copying and/or moving if these operations are clear and meaningful for your type. Otherwise, disable the implicitly generated special functions that perform copies and moves.
   MyClass is neither copyable nor movable:								MyClass(const MyClass&) = delete; MyClass& operator=(const MyClass&) = delete;
Use a struct only for passive objects that carry data; everything else is a class.
Composition is often more appropriate than inheritance. When using inheritance, make it public.
Only very rarely is multiple implementation inheritance actually useful.
Make data members private, unless they are static const
A class definition should usually start with a public: section, followed by protected:, then private:.
When defining a function, parameter order is: inputs, then outputs.
Prefer small and focused functions.
All parameters passed by reference must be labeled const.				void Foo(const string &in, string *out);
Use trailing return types only where using the ordinary syntax (leading return types) is impractical or much less readable. auto foo(int x) -> int;
Prefer to have single, fixed owners for dynamically allocated objects. Prefer to transfer ownership with smart pointers.
   If dynamic allocation is necessary, prefer to keep ownership with the code that allocated it. If other code needs access to the object, consider passing it a copy, or passing a pointer or reference without transferring ownership. Prefer to use std::unique_ptr to make ownership transfer explicit.
Use rvalue references only to define move constructors and move assignment operators, or for perfect forwarding.	void f(string&& s); declares a function whose argument is an rvalue reference to a string.
Use C++-style casts like static_cast<float>(double_value), or brace initialization for conversion of arithmetic types like int64 y = int64{1} << 42. Do not use cast formats like int y = (int)x or int y = int(x)
Use streams only when they are the best tool for the job.
Use prefix form (++i) of the increment and decrement operators with iterators and other template objects.
Use const whenever it makes sense. With C++11, constexpr is a better choice for some uses of const.
Of the built-in C++ integer types, the only one used is int. If a program needs a variable of a different size, use a precise-width integer type from <stdint.h>, such as int16_t.
Use 0 for integers, 0.0 for reals, nullptr for pointers, and '\0' for chars.
Use lambda expressions where appropriate, prefer explicit captures if the lambda may escape the current scope. executor->Schedule([&foo] { Frobnicate(foo); })
   & (implicitly catch the odr-used automatic variables and *this by reference) and
   = (implicitly catch the odr-used automatic variables by copy and implicitly catch *this by reference if this is odr-used)

------------------------------------------------------------------------------------------
https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-interfaces
Abstract/interface: 
class Shape {    // better: Shape is a pure interface
public:
	Shape() = default;
	virtual ~Shape() = default; // If you define at least one virtual function in your class you should also define a virtual destructor.

	virtual Point center() const = 0;   // pure virtual function
	virtual void draw() const = 0;
	virtual void rotate(int) = 0;
	// ...
	// ... no data members ...
};

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-functions
SOLID
If a function is very small and time-critical, declare it inline
If your function may not throw, declare it noexcept

void f(int*); // accepts any int*
void g(unique_ptr<int>); // can only accept ints for which you want to transfer ownership
void g(shared_ptr<int>); // can only accept ints for which you are willing to share ownership
void h(const unique_ptr<int>&); // doesn't change ownership, but requires a particular ownership of the caller
void h(int&); // accepts any int

void thinko(const unique_ptr<widget>); // function owns the lifetime of a object
void thinko(const widget&); // function uses the object
void reseat(unique_ptr<widget>&); // "will" or "might" reseat pointer
void thinko(const unique_ptr<widget>&); // usually not what you want

void share(shared_ptr<widget>);            // share -- "will" retain refcount
void may_share(const shared_ptr<widget>&); // "might" retain refcount
void reseat(shared_ptr<widget>&);          // "might" reseat ptr

unsigned int wsa[5] = {1,2,3,4,5}; // wsa[0] == 1
unsigned int* wsb = new unsigned int[5](); // wsa[0] == 0
unsigned int* wsp = new (wsa) unsigned int[5](); // Use placement new (to use a know piece of memory).
auto wsb = make_unique<unsigned int[]>(5); // Zero-initialized: assert(wsb), wsb[0] == 0

Parameter passing: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f15-prefer-simple-and-conventional-ways-of-passing-information
For “in” parameters, pass cheaply-copied types by value and others by reference to const
For “in-out” parameters, pass by reference to non-const
For “consume” parameters, pass by X&& and std::move the parameter
	void sink(vector<int>&& v) {   // sink takes ownership of whatever the argument owned
		// usually there might be const accesses of v here
		store_somewhere(std::move(v));
		// usually no more use of v here; it is moved-from
	}
For “forward” parameters, pass by TP&& and only std::forward the parameter (std::forward is an enhanced std::move but for templates, coz it has type deduction)
For “out” output values, prefer return values to output parameters
To return multiple “out” values, prefer returning a struct or tuple ( via reference with tie() or value with make_tuple() )
Use a unique_ptr<T> to transfer ownership where a pointer is needed
Prefer T* over T& when “no argument” is a valid option
Return a T* to indicate a position (only)
Return a T& when copy is undesirable and “returning no object” isn’t needed
Don’t return a T&&

Use a lambda when a function won’t do (to capture local variables, or to write a local function)
Prefer capturing by reference in lambdas that will be used locally, including passed to algorithms
Avoid capturing by reference in lambdas that will be used nonlocally, including returned, stored on the heap, or passed to another thread
Where there is a choice, prefer default arguments over overloading // void print(const string& s, format f = {});

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class
Place helper functions in the same namespace as the class they support. This is especially important for overloaded operators.
Use class rather than struct if any member is non-public.
“the rule of zero” - If you can avoid defining default operations, do.
“the rule of five” or six - If you define or =delete any default operation, define or =delete them all: def ctor, copy ctor, copy assignment op, move ctor, move assignment op, dtor 
If a class has an owning pointer member, define a destructor. If a class has an owning reference member, define a destructor.
A base class destructor should be either public and virtual, or protected and nonvirtual.
A destructor may not fail. Declare a destructor noexcept. That will ensure that it either completes normally or terminate the program. "~X() noexcept".
Prefer in-class initializers to member initializers in constructors for constant initializers.
Use delegating constructors to represent common actions for all constructors of a class.
By default, declare single-argument constructors explicit.
Use a factory function if you need “virtual behavior” during initialization.
Ensure that a class has a default constructor.
Avoid protected data. Protected member function can be just fine.
Ensure all non-const data members have the same access level

Prefer class enums over “plain” enums
enum class Month { jan = 1, feb, mar, apr, may, jun, jul, august, sep, oct, nov, dec }; // starting with 1 is conventional
enum class Base_flag { dec = 1, oct = dec << 1, hex = dec << 2 }; // set of bits

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource
Resource Acquisition is Initialization (RAII):
Immediately give the result of an explicit resource allocation to a manager object.
Do not pass a pointer or reference obtained from an aliased smart pointer.
Take smart pointers as parameters only to explicitly express lifetime semantics. 
A function that does not manipulate lifetime should take raw pointers or references instead. // void f(widget& w) vs void f(shared_ptr<widget>& w)
Take a unique_ptr<widget> parameter to express that a function assumes ownership of a widget.
Take a unique_ptr<widget>& parameter to express that a function reseats thew idget.

Take a shared_ptr<widget> parameter to express that a function is part owner.
Take a shared_ptr<widget>& parameter to express that a function might reseat the shared pointer. “reseat” means “making a reference or a smart pointer refer to a different object.”
	void share(shared_ptr<widget>);            // share -- "will" retain refcount
	void reseat(shared_ptr<widget>&);          // "might" reseat ptr
	void may_share(const shared_ptr<widget>&); // "might" retain refcount

Declare an object const or constexpr unless you want to modify its value later on
Use lambdas for complex initialization, especially of const variables: const widget x = [&]{return 1;}();

Use atomic<> types where you might have used volatile in some other language. Use a mutex for more complicated examples.
Use RAII, never plain lock()/unlock(). // unique_lock<mutex> lck {mtx};
  One of the differences between std::lock_guard and std::unique_lock is that the programmer is able to unlock std::unique_lock, but she/he is not able to unlock std::lock_guard. Let’s explain it in more detail.
Prefer gsl::raii_thread over std::thread unless you plan to detach(). raii_thread is a std::thread with a destructor that joined and cannot be detached(). By “OK” we mean that the object will be in scope (“live”) for as long as a thread can use the pointer to it.
Do not pass pointers to local variables to non-raii_threads
Pass small amounts of data between threads by value, rather than by reference or pointer
Share ownership between unrelated threads use shared_ptr
Use a future to return a value from a concurrent task. Use a async() to spawn a concurrent task.
Yield thread to hint to the implementation to reschedule the execution of threads, allowing other threads to run. // while (foo==0) std::this_thread::yield();

Catch exceptions from a hierarchy by reference. 
	catch (exception e) {	// don't: may slice
	catch (exception& e) {	// OK
Use a final_action object to express cleanup if no suitable resource handle is available. // auto _ = finally([p] { free(p); });
If you can’t throw exceptions, consider failing fast. // abort(), quick_exit()

Specify concepts for all template arguments (TODO)
Prefer 'using' over 'typedef' for defining aliases

--- STL https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#sl-the-standard-library
std::basic_string is a class template for making strings out of character types, std::string is a typedef for a specialization of that class template for char
Prefer using STL array or vector instead of a C array
Prefer using STL vector by default unless you have a reason to use a different container
Use std::string_view to refer to character sequences,
Use zstring or czstring to refer to a C-style, zero-terminated, sequence of characters
Use char* to refer to a single character
Use std::byte to refer to byte values that do not necessarily represent characters
Prefer iostreams for I/O
Unless you use printf-family functions call ios_base::sync_with_stdio(false)
Avoid endl // cout << "Hello, World!\n";
<regex> is the standard C++ regular expression library. It supports a variety of regular expression pattern conventions.
<chrono> (defined in namespace std::chrono) provides the notions of time_point and duration together with functions for outputting time in various units. It provides clocks for registering time_points.












*/