#include <iostream>
#include <cstring>

class String {
public:
    String() : m_buffer(nullptr), m_size(0) {
    }

    String(const char* str) : m_buffer(nullptr), m_size(0) {
        if (str) {
            m_size = std::strlen(str);
            m_buffer = new char[m_size + 1];
            std::memcpy(m_buffer, str, m_size + 1);
        }
    }

    String(const String& other) : m_buffer(nullptr), m_size(0) {
        if (other.m_buffer) {
            m_size = other.m_size;
            m_buffer = new char[m_size + 1];
            std::memcpy(m_buffer, other.m_buffer, m_size + 1);
        }
    }

    ~String() {
        delete[] m_buffer;
    }

    size_t size() const {
        return m_size;
    }

    const char* c_str() const {
        return m_buffer;
    }

    char operator[](size_t index) const {
        return m_buffer[index];
    }

    String& operator=(const String& other) {
        if (this != &other) {
            delete[] m_buffer;
            m_buffer = nullptr;
            m_size = 0;

            if (other.m_buffer) {
                m_size = other.m_size;
                m_buffer = new char[m_size + 1];
                std::memcpy(m_buffer, other.m_buffer, m_size + 1);
            }
        }
        return *this;
    }

    String& operator+=(const String& other) {
        if (other.m_buffer) {
            const size_t new_size = m_size + other.m_size;
            char* new_buffer = new char[new_size + 1];

            if (m_buffer) {
                std::memcpy(new_buffer, m_buffer, m_size);
            }
            std::memcpy(new_buffer + m_size, other.m_buffer, other.m_size + 1);

            delete[] m_buffer;
            m_buffer = new_buffer;
            m_size = new_size;
        }

        return *this;
    }

private:
    char* m_buffer;
    size_t m_size;
};

String operator+(const String& lhs, const String& rhs) {
    String result(lhs);
    result += rhs;
    return result;
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}

int main() {
    String s1;
    String s2("hello");
    String s3(s2);
    String s4 = s2;

    s4 += s3;

    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl;

    return 0;
}
