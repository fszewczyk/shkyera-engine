namespace shkyera {

template <typename T> Grid<T>::Grid() : _cellSize(1) {}

template <typename T> Grid<T>::Grid(glm::vec<2, size_t> size) : _cellSize(1), _grid(size[0]) {
    for (auto &plane : _grid) {
        plane.resize(size[1]);
        for (auto &row : plane)
            row.resize(1);
    }
}

template <typename T> Grid<T>::Grid(glm::vec<3, size_t> size) : _cellSize(1), _grid(size[0]) {
    for (auto &plane : _grid) {
        plane.resize(size[1]);
        for (auto &row : plane)
            row.resize(size[2]);
    }
}

template <typename T> Grid<T>::Grid(glm::vec<3, size_t> size, float cellSize) : _cellSize(cellSize), _grid(size[0]) {
    for (auto &plane : _grid) {
        plane.resize(size[1]);
        for (auto &row : plane)
            row.resize(size[2]);
    }
}

template <typename T> T Grid<T>::operator()(size_t x, size_t y, size_t z) const { return _grid[x][y][z]; }

} // namespace shkyera