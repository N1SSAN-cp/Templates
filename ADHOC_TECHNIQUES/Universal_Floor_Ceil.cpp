template <typename T> T floor_div(T a, T b) {
  return a / b - (a % b && (a ^ b) < 0);
}
template <typename T> T ceil_div(T a, T b) { return -floor_div(-a, b); }
