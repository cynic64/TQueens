#include <string_view>
#include <iostream>

struct Nil;

template <typename H, typename T = Nil> struct List {
	typedef H Head;
	typedef T Tail;
};

template <typename T, typename... Ts> struct ListConstruct { typedef List<T, typename ListConstruct<Ts...>::value> value; };
template <typename T> struct ListConstruct<T> { typedef List<T, Nil> value; };

template <typename A, typename B> struct ListConcat { typedef List<typename A::Head, typename ListConcat<typename A::Tail, B>::value> value; };
template <typename B> struct ListConcat<Nil, B> { typedef B value; };
template <typename A> struct ListConcat<A, Nil> { typedef A value; };
template <> struct ListConcat<Nil, Nil> { typedef Nil value; };

template <typename L> struct ListConcatAll { typedef typename ListConcat<typename L::Head, typename ListConcatAll<typename L::Tail>::value>::value value; };
template <> struct ListConcatAll<Nil> { typedef Nil value; };

struct True;
struct False;

template <typename T> struct Not { typedef False value; };
template <> struct Not<False> { typedef True value; };

template <typename A, typename B> struct Or { typedef False value; };
template <typename B> struct Or<True, B> { typedef True value; };
template <typename A> struct Or<A, True> { typedef True value; };
template <> struct Or<True, True> { typedef True value; };

template <typename L> struct AnyTrue;
template <typename T> struct AnyTrue<List<True, T>> { typedef True value; };
template <typename H, typename T> struct AnyTrue<List<H, T>> { typedef typename AnyTrue<T>::value value; };
template <> struct AnyTrue<Nil> { typedef False value; };

struct Z;
template <typename T> struct S;
typedef Z N0;
typedef S<N0> N1;
typedef S<N1> N2;
typedef S<N2> N3;
typedef S<N3> N4;
typedef S<N4> N5;
typedef S<N5> N6;
typedef S<N6> N7;
typedef S<N7> N8;

template <typename X, typename Y> struct Equal { typedef False value; };
template <typename X> struct Equal<X, X> { typedef True value; };

template <typename X, typename Y> struct LessThan;
template <> struct LessThan<Z, Z> { typedef False value; };
template <typename X> struct LessThan<X, Z> { typedef False value; };
template <typename Y> struct LessThan<Z, Y> { typedef True value; };
template <typename X, typename Y> struct LessThan<S<X>, S<Y>> { typedef typename LessThan<X, Y>::value value; };

template <typename X, typename Y> struct AbsDiff;
template <> struct AbsDiff<Z, Z> { typedef Z value; };
template <typename X> struct AbsDiff<X, Z> { typedef X value; };
template <typename Y> struct AbsDiff<Z, Y> { typedef Y value; };
template <typename X, typename Y> struct AbsDiff<S<X>, S<Y>> { typedef typename AbsDiff<X, Y>::value value; };

template <typename X> struct Range;
template <> struct Range<Z> { typedef Nil value; };
template <typename X> struct Range<S<X>> { typedef List<S<X>, typename Range<X>::value> value; };

template <typename F, typename A> struct Apply;

template <typename F, typename L> struct Map { typedef List<typename Apply<F, typename L::Head>::value, typename Map<F, typename L::Tail>::value> value; };
template <typename F> struct Map<F, Nil> { typedef Nil value; };

template <typename F, typename L> struct FlatMap { typedef typename ListConcatAll<typename Map<F, L>::value>::value value; };
template <typename F> struct FlatMap<F, Nil> { typedef Nil value; };

template <typename C, typename X, typename L> struct PrependIf { typedef L value; };
template <typename X, typename L> struct PrependIf<True, X, L> { typedef List<X, L> value; };
template <typename F, typename L> struct Filter { typedef typename PrependIf<typename Apply<F, typename L::Head>::value, typename L::Head, typename Filter<F, typename L::Tail>::value>::value value; };
template <typename F> struct Filter<F, Nil> { typedef Nil value; };

template <typename X, typename Y> struct Queen { typedef X y; typedef Y x; };

template <typename X> struct Queen_;
template <typename X, typename Y> struct Apply<Queen_<X>, Y> { typedef Queen<X, Y> value; };
template <typename X, typename N> struct QueensInRow { typedef typename Map<Queen_<X>, typename Range<N>::value>::value value; };

template <typename A, typename B> struct Threatens {
	typedef typename Equal<typename A::x, typename B::x>::value XEqual;
	typedef typename Equal<typename A::y, typename B::y>::value YEqual;
	typedef typename Or<XEqual, YEqual>::value SameLine;
	typedef typename Equal<typename AbsDiff<typename A::x, typename B::x>::value, typename AbsDiff<typename A::y, typename B::y>::value>::value SameDiag;
	typedef typename Or<SameLine, SameDiag>::value value;
};
template <typename A> struct Threatens_;
template <typename A, typename B> struct Apply<Threatens_<A>, B> { typedef typename Threatens<A, B>::value value; };

template <typename L, typename A> struct Safe { typedef typename Not<typename AnyTrue<typename Map<Threatens_<A>, L>::value>::value>::value value; };
template <typename L> struct Safe_;
template <typename L, typename A> struct Apply<Safe_<L>, A> { typedef typename Safe<L, A>::value value; };

template <typename L> struct Prepend_;
template <typename L, typename X> struct Apply<Prepend_<L>, X> { typedef List<X, L> value; };

template <typename N, typename X, typename L> struct AddQueen { typedef typename Map<Prepend_<L>, typename Filter<Safe_<L>, typename QueensInRow<X, N>::value>::value>::value value; };
template <typename N, typename X> struct AddQueen_;
template <typename N, typename X, typename L> struct Apply<AddQueen_<N, X>, L> { typedef typename AddQueen<N, X, L>::value value; };

template <typename N, typename X, typename L> struct AddQueenToAll { typedef typename FlatMap<AddQueen_<N, X>, L>::value value; };

template <typename C, typename N, typename X, typename L> struct AddQueensIf { typedef L value; };
template <typename N, typename X, typename L> struct AddQueens { typedef typename AddQueensIf<typename LessThan<X, N>::value, N, X, L>::value value; };
template <typename N, typename X, typename L> struct AddQueensIf<True, N, X, L> { typedef typename AddQueens<N, S<X>, typename AddQueenToAll<N, X, L>::value>::value value; };

template <typename N> struct Solution { typedef typename AddQueens<N, Z, List<Nil, Nil>>::value::Head value; };

struct S_;
template <typename X> struct Apply<S_, X> { typedef S<X> value; };
struct Range_;
template <typename X> struct Apply<Range_, X> { typedef typename Range<X>::value value; };
template <typename X> struct GreaterThanFour { typedef typename LessThan<N4, X>::value value; };
struct GreaterThanFour_;
template <typename X> struct Apply<GreaterThanFour_, X> { typedef typename GreaterThanFour<X>::value value; };

//-----------------------

template <typename T>
constexpr auto type_name() noexcept {
  std::string_view name = "Error: unsupported compiler", prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void) noexcept";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

int main() {
	typedef ListConstruct<N0, N1, N2, N3>::value X;
	std::cout << "X: " << type_name<X>() << std::endl;

	typedef ListConstruct<bool, bool, bool>::value Y;
	std::cout << "Y: " << type_name<Y>() << std::endl;

	typedef ListConstruct<float, char>::value W;
	std::cout << "W: " << type_name<W>() << std::endl;

	typedef ListConstruct<X, Y, W>::value ListOfLists;
	typedef ListConcatAll<ListOfLists>::value Flattened;
	std::cout << "X . Y . W: " << type_name<Flattened>() << std::endl;

	std::cout << "Not(AnyTrue): " << type_name<Not<AnyTrue<ListConstruct<False, False, False, True>::value>::value>::value>() << std::endl;

	std::cout << "Or(False, True): " << type_name<Or<False, True>::value>() << std::endl;

        std::cout << "S<N6> == N7: " << type_name<Equal<S<N6>, N7>::value>() << std::endl;

        std::cout << "Z < N1: " << type_name<LessThan<Z, N1>::value>() << std::endl;
        std::cout << "Z < Z: " << type_name<LessThan<Z, Z>::value>() << std::endl;
        std::cout << "N1 < N8: " << type_name<LessThan<N1, N8>::value>() << std::endl;
        std::cout << "N8 < N7: " << type_name<LessThan<N8, N7>::value>() << std::endl;

        std::cout << "|N4 - N8|: " << type_name<AbsDiff<N8, N4>::value>() << std::endl;
        std::cout << "|Z - Z|: " << type_name<AbsDiff<Z, Z>::value>() << std::endl;

        std::cout << "0..4: " << type_name<Range<N4>::value>() << std::endl;

	std::cout << "S(0..4): " << type_name<Map<S_, Range<N4>::value>::value>() << std::endl;

	std::cout << "Range(0..3): " << type_name<FlatMap<Range_, Range<N3>::value>::value>() << std::endl;
	std::cout << "Range(0..7) where X>4: " << type_name<Filter<GreaterThanFour_, Range<N7>::value>::value>() << std::endl;
	std::cout << "QueensInRow(N2, N5): " << type_name<QueensInRow<N2, N5>::value>() << std::endl;
	std::cout << "(1, 4) threatens (2, 3): " << type_name<Threatens<Queen<N1, N4>, Queen<N2, N3>>::value>() << std::endl;
	std::cout << "(1, 4) threatens (2, 6): " << type_name<Threatens<Queen<N1, N4>, Queen<N2, N6>>::value>() << std::endl;

	typedef typename ListConstruct<Queen<N1, N4>, Queen<N2, N6>>::value Config;
	std::cout << "(3, 1) is safe: " << type_name<Safe<Config, Queen<N3, N1>>::value>() << std::endl;

	std::cout << "Possible queens at X=3 on ((1, 4), (2, 6)) for 6x6: " << type_name<AddQueen<N6, N3, Config>::value>() << std::endl;

	std::cout << "Solution for N8: " << type_name<Solution<N8>::value>() << std::endl;
};
