#pragma once

#include "Mixture/Core/Base.hpp"

namespace Mixture
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		std::filesystem::path GetShaderPath() const;

	private:
		std::filesystem::path m_AssetsPath;
	};
}