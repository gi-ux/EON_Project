//
// Created by Thefefo3 on 20/04/2021.
//

#include <vector>
#include <memory>
#include <cstddef>

class NDArray {
    std::vector<size_t> m_dims, m_strides;
    std::unique_ptr<float[]> m_buf;

public:
    NDArray(std::vector<size_t> dims):
            m_dims{std::move(dims)}
    {
        m_strides.resize(m_dims.size());
        size_t stride = 1;
        for (int i = m_dims.size() - 1; i >= 0; -- i) {
            m_strides[i] = stride;
            stride *= m_dims[i];
        }
        m_buf.reset(new float[stride]);
    }

    float& operator[] (std::initializer_list<size_t> idx) {
        size_t offset = 0;
        auto stride = m_strides.begin();
        for (auto i: idx) {
            offset += i * *stride;
            ++ stride;
        }
        return m_buf[offset];
    }
};