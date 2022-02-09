template<typename CharType>
class StringViewBase {
    public:
    template<size_t LENGTH>
    constexpr StringViewBase(const CharType(&str)[LENGTH]) 
    : m_string(str),
      m_length(LENGTH) {}

    constexpr StringViewBase(const StringBase<CharType>& str)
    : m_string(str.c_str()),
      m_length(str.length()) {}

    constexpr CharType operator[]( unsigned int index ) const
    {
        if (index >= m_length || m_length == 0) return 0;
        return m_string[index];
    }

    private:
    const CharType* m_string{nullptr};
    size_t m_length{0};
};

using StringView = StringViewBase<char>;
