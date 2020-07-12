#pragma once

namespace UI
{
	template<typename T>
	class IIterable
	{
	public:
		virtual const T& next(const T& value) const = 0;

		virtual ~IIterable()
		{

		}
	};
}