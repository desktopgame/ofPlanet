#ifndef GEL_GSYSTEM_SCORETABLE_HPP
#define GEL_GSYSTEM_SCORETABLE_HPP
#include <algorithm>
#include <array>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>
namespace gel {
/**
 * ScoreTable is ranking of score.
 * @type D
 * - required[STATIC MEMBER]	compare(a, b)
 * - required[STATIC MEMBER]	write(data)
 * - required[STATIC MEMBER]	read(line)
 */
template <typename D, int N>
class ScoreTable {
       public:
        explicit ScoreTable();
        /**
         * clear all score.
         */
        void reset();
        /**
         * insert a new data
         * @param data
         */
        void insert(const D data);
        /**
         * return a data by rank.
         * @return
         */
        template <int Index>
        std::optional<D> at() const;

        /**
         * @param index
         * @return
         */
        std::optional<D> at(int index) const;

        /**
         * @param filename
         * @return
         */
        static ScoreTable<D, N> read(const std::string& filename);

        /**
         * @param filename
         * @param data
         */
        static void write(const std::string& filename,
                          const ScoreTable<D, N>& data);

       private:
        std::array<std::optional<D>, N> table;
};

template <typename D, int N>
inline ScoreTable<D, N>::ScoreTable() : table() {
        reset();
}

template <typename D, int N>
inline void ScoreTable<D, N>::reset() {
        std::fill(table.begin(), table.end(), std::nullopt);
}

template <typename D, int N>
inline void ScoreTable<D, N>::insert(const D data) {
        int r = N;
        for (int i = 0; i < N; i++) {
                if (!table[i]) {
                        table[i] = data;
                        r = i;
                        break;
                }
        }
        if (r == N) {
                for (int i = N - 1; i >= 0; i++) {
                        // if(data.value > ev) {
                        if (D::compare(data, *table[i]) > 0) {
                                for (int j = N - 1; j > i; j--) {
                                        table[j - 1] = table[j];
                                }
                                table[i] = data;
                                break;
                        }
                }
        }
        std::sort(table.begin(), table.end(), [](auto a, auto b) {
                if (!a && !b) {
                        return true;
                }
                if (!a) {
                        return false;
                }
                if (!b) {
                        return false;
                }
                // return (*a).value > (*b).value;
                return (D::compare(*a, *b)) > 0;
        });
}

template <typename D, int N>
template <int Index>
inline std::optional<D> ScoreTable<D, N>::at() const {
        return table[Index];
}

template <typename D, int N>
std::optional<D> ScoreTable<D, N>::at(int index) const {
        return table[index];
}

template <typename D, int N>
inline ScoreTable<D, N> ScoreTable<D, N>::read(const std::string& filename) {
        ScoreTable<D, N> rdata;
        std::ifstream ifs(filename);
        // read item count
        std::string line;
        std::getline(ifs, line);
        int items = std::stoi(line);
        int icount = 0;
        if (items != N) {
                throw std::logic_error("invalid item count");
        }
        // read items
        while (!ifs.eof()) {
                std::getline(ifs, line);
                // D required |D D::read(const std::string& line)|
                if (line == "NULL") {
                        rdata.table[icount] = std::nullopt;
                } else {
                        D data = D::read(line);
                        rdata.table[icount] = data;
                }

                icount++;
                if (icount == items) {
                        break;
                }
        }
        return rdata;
}
template <typename D, int N>
inline void ScoreTable<D, N>::write(const std::string& filename,
                                    const ScoreTable<D, N>& data) {
        std::ofstream ofs(filename);
        ofs << N << std::endl;
        for (int i = 0; i < N; i++) {
                // D required |std::string D::write(const D& data)|
                if (data.table[i]) {
                        ofs << D::write(*(data.table[i])) << std::endl;
                } else {
                        ofs << "NULL" << std::endl;
                }
        }
}

template <typename T>
struct NumericScore {
        T value;
        // converting constructor
        NumericScore(const T value);
        static int compare(const NumericScore<T>& a, const NumericScore<T>& b);
        static std::string write(const NumericScore<T>& data);
        static NumericScore<T> read(const std::string& line);
};
template <typename T>
inline NumericScore<T>::NumericScore(const T value) : value(value) {}

template <typename T>
inline int NumericScore<T>::compare(const NumericScore<T>& a,
                                    const NumericScore<T>& b) {
        return a.value - b.value;
}

template <typename T>
inline std::string NumericScore<T>::write(const NumericScore<T>& data) {
        return std::to_string(data.value);
}

template <>
inline NumericScore<int> NumericScore<int>::read(const std::string& line) {
        NumericScore<int> d(0);
        d.value = std::stoi(line);
        return d;
}
template <>
inline NumericScore<float> NumericScore<float>::read(const std::string& line) {
        NumericScore<float> d(0);
        d.value = std::stof(line);
        return d;
}
template <>
inline NumericScore<double> NumericScore<double>::read(
    const std::string& line) {
        NumericScore<double> d(0);
        d.value = std::stod(line);
        return d;
}
using IntegerScore = NumericScore<int>;
using FloatScore = NumericScore<float>;
using DoubleScore = NumericScore<double>;
}  // namespace gel
#endif