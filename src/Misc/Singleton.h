#pragma once

template <class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if(!m_pInstance)
		{
			m_pInstance = new T{};
		}
		return m_pInstance;
	}

	void Destroy() const
	{
		delete m_pInstance;
	}
private:
	inline static T* m_pInstance{nullptr};
};