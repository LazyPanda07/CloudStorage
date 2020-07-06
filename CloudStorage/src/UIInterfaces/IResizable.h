#pragma once

namespace UI
{
	class IResizable
	{
	public:
		virtual void resize() = 0;

		virtual ~IResizable()
		{

		}
	};
}