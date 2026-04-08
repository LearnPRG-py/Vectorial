#ifndef VECTORIAL_H
#define VECTORIAL_H

enum appendType { cycle, ignore };
inline int min(int a, int b) { return (a < b) ? a : b; }

template <typename T, int N>
struct Vector {
  private:
      T data[N];
      int entriesAdded = 0;

  public:
  void push_back(T element, appendType type = appendType::cycle) {
    if (type == cycle) {
        int latest = entriesAdded % N;
        data[latest] = element;
        entriesAdded++;
    }
  }

  // checks if buffer is empty

  bool is_empty() {
    return entriesAdded == 0;
  }

  // checks if buffer is full

  bool is_full() {
    return entriesAdded == N;
  }

  T operator[](int idx) const {
    return data[(entriesAdded - 1 - idx + 2*N) % N]; 
  }
  int size() const { return min(entriesAdded, N); }
  void reset() { 
    entriesAdded = 0; 
    for (int i = 0; i < N; i++) {
      data[i] = T{};
    }
  }
};

#endif
