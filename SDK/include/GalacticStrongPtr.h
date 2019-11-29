#pragma once

#include <assert.h>

namespace Galactic3D
{
	// Strong reference
	template<class T> class Strong
	{
	public:
		Strong()
		{
			m_pRef = nullptr;
		}

		Strong(T* pStrongable)
		{
			m_pRef = pStrongable;
			if (m_pRef != nullptr)
				m_pRef->AddRef();
		}

		template<class Y>
		Strong(const Strong<Y>& r)
		{
			m_pRef = r.GetPointer();
			if (m_pRef != nullptr)
				m_pRef->AddRef();
		}

		Strong(const Strong<T>& r)
		{
			m_pRef = r.m_pRef;
			if (m_pRef != nullptr)
				m_pRef->AddRef();
		}

		~Strong()
		{
			if (m_pRef != nullptr)
				m_pRef->Release();
		}

	protected:
		T* m_pRef;

	public:
		static Strong New(T* r)
		{
			Strong<T> Ref;
			Ref.m_pRef = r;
			return Ref;
		}

		template<class Y>
		static Strong New(Y* r)
		{
			Strong<T> Ref;
			Ref.m_pRef = static_cast<T*>(r);
			return Ref;
		}

		template<class Y>
		Strong& operator=(Y* r)
		{
			if (m_pRef != r)
			{
				if (m_pRef != nullptr)
					m_pRef->Release();
				m_pRef = r;
				if (m_pRef != nullptr)
					m_pRef->AddRef();
			}
			return *this;
		}

		template<class Y>
		Strong& operator=(const Strong<Y>& r)
		{
			if (m_pRef != r.GetPointer())
			{
				if (m_pRef != nullptr)
					m_pRef->Release();
				m_pRef = r.GetPointer();
				if (m_pRef != nullptr)
					m_pRef->AddRef();
			}
			return *this;
		}

		Strong& operator=(const Strong<T>& r)
		{
			if (m_pRef != r.GetPointer())
			{
				if (m_pRef != nullptr)
					m_pRef->Release();
				m_pRef = r.GetPointer();
				if (m_pRef != nullptr)
					m_pRef->AddRef();
			}
			return *this;
		}

		template<class Y>
		inline Strong<Y> StaticCast() const
		{
			if (m_pRef != nullptr)
			{
				return Strong<Y>(static_cast<Y*>(m_pRef));
			}
			return Strong<Y>();
		}

		inline operator T*() const { return static_cast<T*>(m_pRef); }
		inline T& operator*() const { assert(m_pRef); return *static_cast<T*>(m_pRef); }
		inline T** operator&() { assert(IsNull()); return &m_pRef; }
		inline T** operator&() const { assert(IsNull()); return &m_pRef; }
		inline T* operator->() const { assert(m_pRef); return static_cast<T*>(m_pRef); }
		inline T* GetPointer() const { return static_cast<T*>(m_pRef); }
		inline bool IsNull() const { return m_pRef == nullptr; }
		inline void SetNull() {
			if (m_pRef != nullptr)
				m_pRef->Release();
			m_pRef = nullptr;
		}
	};

	template<class T, class U> inline bool operator==(Strong<T> const& a, Strong<U> const& b)
	{
		return a.GetPointer() == b.GetPointer();
	}

	template<class T, class U> inline bool operator!=(Strong<T> const& a, Strong<U> const& b)
	{
		return a.GetPointer() != b.GetPointer();
	}
};
