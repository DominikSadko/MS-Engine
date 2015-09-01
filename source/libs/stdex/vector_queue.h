#ifndef __VECTOR_QUEUE_H__
#define __VECTOR_QUEUE_H__

namespace stdex
{
template <typename Type, typename Compare = std::less<Type>>
class vector_queue
{
	public:
		explicit vector_queue(const Compare& compare = Compare())
			: m_compare{compare}
		{ }

		void push(Type element)
		{
			m_elements.push_back(std::move(element));
			std::push_heap(m_elements.begin(), m_elements.end(), m_compare);
		}

		void pop()
		{
			std::pop_heap(m_elements.begin(), m_elements.end(), m_compare);
			m_elements.pop_back();
		}

		Type top() const { return m_elements.front(); }
		bool empty() const { return m_elements.empty(); }

		std::vector<Type>& elements() { return m_elements; }

	private:
		std::vector<Type> m_elements;
		Compare m_compare;
};
} // stdex

#endif // __VECTOR_QUEUE_H__ //
