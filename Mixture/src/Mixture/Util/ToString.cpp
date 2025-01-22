#include "mxpch.hpp"
#include "Mixture/Util/ToString.hpp"

namespace Mixture::Vulkan::ToString
{
    std::string_view Format(VkFormat format)
    {
        switch (format)
        {
            case VK_FORMAT_UNDEFINED: return "Undefined";
            case VK_FORMAT_R4G4_UNORM_PACK8: return "R4G4 UNorm Pack8";
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return "R4G4B4A4 UNorm Pack16";
            case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return "B4G4R4A4 UNorm Pack16";
            case VK_FORMAT_R5G6B5_UNORM_PACK16: return "R5G6B5 UNorm Pack16";
            case VK_FORMAT_B5G6R5_UNORM_PACK16: return "B5G6R5 UNorm Pack16";
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return "R5G5B5A1 UNorm Pack16";
            case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return "B5G5R5A1 UNorm Pack16";
            case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return "A1R5G5B5 UNorm Pack16";
            case VK_FORMAT_R8_UNORM: return "R8 UNorm";
            case VK_FORMAT_R8_SNORM: return "R8 SNorm";
            case VK_FORMAT_R8_USCALED: return "R8 UScaled";
            case VK_FORMAT_R8_SSCALED: return "R8 SScaled";
            case VK_FORMAT_R8_UINT: return "R8 UInt";
            case VK_FORMAT_R8_SINT: return "R8 SInt";
            case VK_FORMAT_R8_SRGB: return "R8 SRGB";
            case VK_FORMAT_R8G8_UNORM: return "R8G8 UNorm";
            case VK_FORMAT_R8G8_SNORM: return "R8G8 SNorm";
            case VK_FORMAT_R8G8_USCALED: return "R8G8 UScaled";
            case VK_FORMAT_R8G8_SSCALED: return "R8G8 SScaled";
            case VK_FORMAT_R8G8_UINT: return "R8G8 UInt";
            case VK_FORMAT_R8G8_SINT: return "R8G8 SInt";
            case VK_FORMAT_R8G8_SRGB: return "R8G8 SRGB";
            case VK_FORMAT_R8G8B8_UNORM: return "R8G8B8 UNorm";
            case VK_FORMAT_R8G8B8_SNORM: return "R8G8B8 SNorm";
            case VK_FORMAT_R8G8B8_USCALED: return "R8G8B8 UScaled";
            case VK_FORMAT_R8G8B8_SSCALED: return "R8G8B8 SScaled";
            case VK_FORMAT_R8G8B8_UINT: return "R8G8B8 UInt";
            case VK_FORMAT_R8G8B8_SINT: return "R8G8B8 SInt";
            case VK_FORMAT_R8G8B8_SRGB: return "R8G8B8 SRGB";
            case VK_FORMAT_B8G8R8_UNORM: return "B8G8R8 UNorm";
            case VK_FORMAT_B8G8R8_SNORM: return "B8G8R8 SNorm";
            case VK_FORMAT_B8G8R8_USCALED: return "B8G8R8 UScaled";
            case VK_FORMAT_B8G8R8_SSCALED: return "B8G8R8 SScaled";
            case VK_FORMAT_B8G8R8_UINT: return "B8G8R8 UInt";
            case VK_FORMAT_B8G8R8_SINT: return "B8G8R8 SInt";
            case VK_FORMAT_B8G8R8_SRGB: return "B8G8R8 SRGB";
            case VK_FORMAT_R8G8B8A8_UNORM: return "R8G8B8A8 UNorm";
            case VK_FORMAT_R8G8B8A8_SNORM: return "R8G8B8A8 SNorm";
            case VK_FORMAT_R8G8B8A8_USCALED: return "R8G8B8A8 UScaled";
            case VK_FORMAT_R8G8B8A8_SSCALED: return "R8G8B8A8 SScaled";
            case VK_FORMAT_R8G8B8A8_UINT: return "R8G8B8A8 UInt";
            case VK_FORMAT_R8G8B8A8_SINT: return "R8G8B8A8 SInt";
            case VK_FORMAT_R8G8B8A8_SRGB: return "R8G8B8A8 SRGB";
            case VK_FORMAT_B8G8R8A8_UNORM: return "B8G8R8A8 UNorm";
            case VK_FORMAT_B8G8R8A8_SNORM: return "B8G8R8A8 SNorm";
            case VK_FORMAT_B8G8R8A8_USCALED: return "B8G8R8A8 UScaled";
            case VK_FORMAT_B8G8R8A8_SSCALED: return "B8G8R8A8 SScaled";
            case VK_FORMAT_B8G8R8A8_UINT: return "B8G8R8A8 UInt";
            case VK_FORMAT_B8G8R8A8_SINT: return "B8G8R8A8 SInt";
            case VK_FORMAT_B8G8R8A8_SRGB: return "B8G8R8A8 SRGB";
            case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return "A8B8G8R8 UNorm Pack32";
            case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return "A8B8G8R8 SNorm Pack32";
            case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return "A8B8G8R8 UScaled Pack32";
            case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return "A8B8G8R8 SScaled Pack32";
            case VK_FORMAT_A8B8G8R8_UINT_PACK32: return "A8B8G8R8 UInt Pack32";
            case VK_FORMAT_A8B8G8R8_SINT_PACK32: return "A8B8G8R8 SInt Pack32";
            case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return "A8B8G8R8 SRGB Pack32";
            case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return "A2R10G10B10 UNorm Pack32";
            case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return "A2R10G10B10 SNorm Pack32";
            case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return "A2R10G10B10 UScaled Pack32";
            case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return "A2R10G10B10 SScaled Pack32";
            case VK_FORMAT_A2R10G10B10_UINT_PACK32: return "A2R10G10B10 UInt Pack32";
            case VK_FORMAT_A2R10G10B10_SINT_PACK32: return "A2R10G10B10 SInt Pack32";
            case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return "A2B10G10R10 UNorm Pack32";
            case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return "A2B10G10R10 SNorm Pack32";
            case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return "A2B10G10R10 UScaled Pack32";
            case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return "A2B10G10R10 SScaled Pack32";
            case VK_FORMAT_A2B10G10R10_UINT_PACK32: return "A2B10G10R10 UInt Pack32";
            case VK_FORMAT_A2B10G10R10_SINT_PACK32: return "A2B10G10R10 SInt Pack32";
            case VK_FORMAT_R16_UNORM: return "R16 UNorm";
            case VK_FORMAT_R16_SNORM: return "R16 SNorm";
            case VK_FORMAT_R16_USCALED: return "R16 UScaled";
            case VK_FORMAT_R16_SSCALED: return "R16 SScaled";
            case VK_FORMAT_R16_UINT: return "R16 UInt";
            case VK_FORMAT_R16_SINT: return "R16 SInt";
            case VK_FORMAT_R16_SFLOAT: return "R16 SFloat";
            case VK_FORMAT_R16G16_UNORM: return "R16G16 UNorm";
            case VK_FORMAT_R16G16_SNORM: return "R16G16 SNorm";
            case VK_FORMAT_R16G16_USCALED: return "R16G16 UScaled";
            case VK_FORMAT_R16G16_SSCALED: return "R16G16 SScaled";
            case VK_FORMAT_R16G16_UINT: return "R16G16 UInt";
            case VK_FORMAT_R16G16_SINT: return "R16G16 SInt";
            case VK_FORMAT_R16G16_SFLOAT: return "R16G16 SFloat";
            case VK_FORMAT_R16G16B16_UNORM: return "R16G16B16 UNorm";
            case VK_FORMAT_R16G16B16_SNORM: return "R16G16B16 SNorm";
            case VK_FORMAT_R16G16B16_USCALED: return "R16G16B16 UScaled";
            case VK_FORMAT_R16G16B16_SSCALED: return "R16G16B16 SScaled";
            case VK_FORMAT_R16G16B16_UINT: return "R16G16B16 UInt";
            case VK_FORMAT_R16G16B16_SINT: return "R16G16B16 SInt";
            case VK_FORMAT_R16G16B16_SFLOAT: return "R16G16B16 SFloat";
            case VK_FORMAT_R16G16B16A16_UNORM: return "R16G16B16A16 UNorm";
            case VK_FORMAT_R16G16B16A16_SNORM: return "R16G16B16A16 SNorm";
            case VK_FORMAT_R16G16B16A16_USCALED: return "R16G16B16A16 UScaled";
            case VK_FORMAT_R16G16B16A16_SSCALED: return "R16G16B16A16 SScaled";
            case VK_FORMAT_R16G16B16A16_UINT: return "R16G16B16A16 UInt";
            case VK_FORMAT_R16G16B16A16_SINT: return "R16G16B16A16 SInt";
            case VK_FORMAT_R16G16B16A16_SFLOAT: return "R16G16B16A16 SFloat";
            case VK_FORMAT_R32_UINT: return "R32 UInt";
            case VK_FORMAT_R32_SINT: return "R32 SInt";
            case VK_FORMAT_R32_SFLOAT: return "R32 SFloat";
            case VK_FORMAT_R32G32_UINT: return "R32G32 UInt";
            case VK_FORMAT_R32G32_SINT: return "R32G32 SInt";
            case VK_FORMAT_R32G32_SFLOAT: return "R32G32 SFloat";
            case VK_FORMAT_R32G32B32_UINT: return "R32G32B32 UInt";
            case VK_FORMAT_R32G32B32_SINT: return "R32G32B32 SInt";
            case VK_FORMAT_R32G32B32_SFLOAT: return "R32G32B32 SFloat";
            case VK_FORMAT_R32G32B32A32_UINT: return "R32G32B32A32 UInt";
            case VK_FORMAT_R32G32B32A32_SINT: return "R32G32B32A32 SInt";
            case VK_FORMAT_R32G32B32A32_SFLOAT: return "R32G32B32A32 SFloat";
            case VK_FORMAT_R64_UINT: return "R64 UInt";
            case VK_FORMAT_R64_SINT: return "R64 SInt";
            case VK_FORMAT_R64_SFLOAT: return "R64 SFloat";
            case VK_FORMAT_R64G64_UINT: return "R64G64 UInt";
            case VK_FORMAT_R64G64_SINT: return "R64G64 SInt";
            case VK_FORMAT_R64G64_SFLOAT: return "R64G64 SFloat";
            case VK_FORMAT_R64G64B64_UINT: return "R64G64B64 UInt";
            case VK_FORMAT_R64G64B64_SINT: return "R64G64B64 SInt";
            case VK_FORMAT_R64G64B64_SFLOAT: return "R64G64B64 SFloat";
            case VK_FORMAT_R64G64B64A64_UINT: return "R64G64B64A64 UInt";
            case VK_FORMAT_R64G64B64A64_SINT: return "R64G64B64A64 SInt";
            case VK_FORMAT_R64G64B64A64_SFLOAT: return "R64G64B64A64 SFloat";
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return "B10G11R11 UFloat Pack32";
            case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return "E5B9G9R9 UFloat Pack32";
            case VK_FORMAT_D16_UNORM: return "D16 UNorm";
            case VK_FORMAT_X8_D24_UNORM_PACK32: return "X8 D24 UNorm Pack32";
            case VK_FORMAT_D32_SFLOAT: return "D32 SFloat";
            case VK_FORMAT_S8_UINT: return "S8 UInt";
            case VK_FORMAT_D16_UNORM_S8_UINT: return "D16 UNorm S8 UInt";
            case VK_FORMAT_D24_UNORM_S8_UINT: return "D24 UNorm S8 UInt";
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return "D32 SFloat S8 UInt";
            case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return "BC1 Rgb UNorm Block";
            case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return "BC1 Rgb SRGB Block";
            case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return "BC1 Rgba UNorm Block";
            case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return "BC1 Rgba SRGB Block";
            case VK_FORMAT_BC2_UNORM_BLOCK: return "BC2 UNorm Block";
            case VK_FORMAT_BC2_SRGB_BLOCK: return "BC2 SRGB Block";
            case VK_FORMAT_BC3_UNORM_BLOCK: return "BC3 UNorm Block";
            case VK_FORMAT_BC3_SRGB_BLOCK: return "BC3 SRGB Block";
            case VK_FORMAT_BC4_UNORM_BLOCK: return "BC4 UNorm Block";
            case VK_FORMAT_BC4_SNORM_BLOCK: return "BC4 SNorm Block";
            case VK_FORMAT_BC5_UNORM_BLOCK: return "BC5 UNorm Block";
            case VK_FORMAT_BC5_SNORM_BLOCK: return "BC5 SNorm Block";
            case VK_FORMAT_BC6H_UFLOAT_BLOCK: return "BC6h UFloat Block";
            case VK_FORMAT_BC6H_SFLOAT_BLOCK: return "BC6h SFloat Block";
            case VK_FORMAT_BC7_UNORM_BLOCK: return "BC7 UNorm Block";
            case VK_FORMAT_BC7_SRGB_BLOCK: return "BC7 SRGB Block";
            case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return "Etc2 R8G8B8 UNorm Block";
            case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return "Etc2 R8G8B8 SRGB Block";
            case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return "Etc2 R8G8B8A1 UNorm Block";
            case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return "Etc2 R8G8B8A1 SRGB Block";
            case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return "Etc2 R8G8B8A8 UNorm Block";
            case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return "Etc2 R8G8B8A8 SRGB Block";
            case VK_FORMAT_EAC_R11_UNORM_BLOCK: return "Eac R11 UNorm Block";
            case VK_FORMAT_EAC_R11_SNORM_BLOCK: return "Eac R11 SNorm Block";
            case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return "Eac R11G11 UNorm Block";
            case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return "Eac R11G11 SNorm Block";
            case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return "ASTC 4x4 UNorm Block";
            case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return "ASTC 4x4 SRGB Block";
            case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return "ASTC 5x4 UNorm Block";
            case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return "ASTC 5x4 SRGB Block";
            case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return "ASTC 5x5 UNorm Block";
            case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return "ASTC 5x5 SRGB Block";
            case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return "ASTC 6x5 UNorm Block";
            case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return "ASTC 6x5 SRGB Block";
            case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return "ASTC 6x6 UNorm Block";
            case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return "ASTC 6x6 SRGB Block";
            case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return "ASTC 8x5 UNorm Block";
            case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return "ASTC 8x5 SRGB Block";
            case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return "ASTC 8x6 UNorm Block";
            case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return "ASTC 8x6 SRGB Block";
            case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return "ASTC 8x8 UNorm Block";
            case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return "ASTC 8x8 SRGB Block";
            case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return "ASTC 10x5 UNorm Block";
            case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return "ASTC 10x5 SRGB Block";
            case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return "ASTC 10x6 UNorm Block";
            case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return "ASTC 10x6 SRGB Block";
            case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return "ASTC 10x8 UNorm Block";
            case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return "ASTC 10x8 SRGB Block";
            case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return "ASTC 10x10 UNorm Block";
            case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return "ASTC 10x10 SRGB Block";
            case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return "ASTC 12x10 UNorm Block";
            case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return "ASTC 12x10 SRGB Block";
            case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return "ASTC 12x12 UNorm Block";
            case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return "ASTC 12x12 SRGB Block";
            case VK_FORMAT_G8B8G8R8_422_UNORM: return "G8B8G8R8 422 UNorm";
            case VK_FORMAT_B8G8R8G8_422_UNORM: return "B8G8R8G8 422 UNorm";
            case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM: return "G8 B8 R8 3Plane 420 UNorm";
            case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM: return "G8 B8R8 2Plane 420 UNorm";
            case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM: return "G8 B8 R8 3Plane 422 UNorm";
            case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM: return "G8 B8R8 2Plane 422 UNorm";
            case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM: return "G8 B8 R8 3Plane 444 UNorm";
            case VK_FORMAT_R10X6_UNORM_PACK16: return "R10x6 UNorm Pack16";
            case VK_FORMAT_R10X6G10X6_UNORM_2PACK16: return "R10x6G10x6 UNorm 2Pack16";
            case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16: return "R10x6G10x6B10x6A10x6 UNorm 4Pack16";
            case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16: return "G10x6B10x6G10x6R10x6 422 UNorm 4Pack16";
            case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16: return "B10x6G10x6R10x6G10x6 422 UNorm 4Pack16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16: return "G10x6 B10x6 R10x6 3Plane 420 UNorm 3Pack16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16: return "G10x6 B10x6R10x6 2Plane 420 UNorm 3Pack16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16: return "G10x6 B10x6 R10x6 3Plane 422 UNorm 3Pack16";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16: return "G10x6 B10x6R10x6 2Plane 422 UNorm 3Pack16";
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16: return "G10x6 B10x6 R10x6 3Plane 444 UNorm 3Pack16";
            case VK_FORMAT_R12X4_UNORM_PACK16: return "R12x4 UNorm Pack16";
            case VK_FORMAT_R12X4G12X4_UNORM_2PACK16: return "R12x4G12x4 UNorm 2Pack16";
            case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16: return "R12x4G12x4B12x4A12x4 UNorm 4Pack16";
            case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16: return "G12x4B12x4G12x4R12x4 422 UNorm 4Pack16";
            case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16: return "B12x4G12x4R12x4G12x4 422 UNorm 4Pack16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16: return "G12x4 B12x4 R12x4 3Plane 420 UNorm 3Pack16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16: return "G12x4 B12x4R12x4 2Plane 420 UNorm 3Pack16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16: return "G12x4 B12x4 R12x4 3Plane 422 UNorm 3Pack16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16: return "G12x4 B12x4R12x4 2Plane 422 UNorm 3Pack16";
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16: return "G12x4 B12x4 R12x4 3Plane 444 UNorm 3Pack16";
            case VK_FORMAT_G16B16G16R16_422_UNORM: return "G16B16G16R16 422 UNorm";
            case VK_FORMAT_B16G16R16G16_422_UNORM: return "B16G16R16G16 422 UNorm";
            case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM: return "G16 B16 R16 3Plane 420 UNorm";
            case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM: return "G16 B16R16 2Plane 420 UNorm";
            case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM: return "G16 B16 R16 3Plane 422 UNorm";
            case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM: return "G16 B16R16 2Plane 422 UNorm";
            case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM: return "G16 B16 R16 3Plane 444 UNorm";
            case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM: return "G8 B8R8 2Plane 444 UNorm";
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16: return "G10x6 B10x6R10x6 2Plane 444 UNorm 3Pack16";
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16: return "G12x4 B12x4R12x4 2Plane 444 UNorm 3Pack16";
            case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM: return "G16 B16R16 2Plane 444 UNorm";
            case VK_FORMAT_A4R4G4B4_UNORM_PACK16: return "A4R4G4B4 UNorm Pack16";
            case VK_FORMAT_A4B4G4R4_UNORM_PACK16: return "A4B4G4R4 UNorm Pack16";
            case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK: return "ASTC 4x4 SFloat Block";
            case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK: return "ASTC 5x4 SFloat Block";
            case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK: return "ASTC 5x5 SFloat Block";
            case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK: return "ASTC 6x5 SFloat Block";
            case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK: return "ASTC 6x6 SFloat Block";
            case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK: return "ASTC 8x5 SFloat Block";
            case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK: return "ASTC 8x6 SFloat Block";
            case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK: return "ASTC 8x8 SFloat Block";
            case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK: return "ASTC 10x5 SFloat Block";
            case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK: return "ASTC 10x6 SFloat Block";
            case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK: return "ASTC 10x8 SFloat Block";
            case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK: return "ASTC 10x10 SFloat Block";
            case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK: return "ASTC 12x10 SFloat Block";
            case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK: return "ASTC 12x12 SFloat Block";
            case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: return "PVRTC1 2BPP UNorm Block Image";
            case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: return "PVRTC1 4BPP UNorm Block Image";
            case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: return "PVRTC2 2BPP UNorm Block Image";
            case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: return "PVRTC2 4BPP UNorm Block Image";
            case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: return "PVRTC1 2BPP SRGB Block Image";
            case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: return "PVRTC1 4BPP SRGB Block Image";
            case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: return "PVRTC2 2BPP SRGB Block Image";
            case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: return "PVRTC2 4BPP SRGB Block Image";
            case VK_FORMAT_R16G16_SFIXED5_NV: return "R16G16 Sfixed5 Nv";
            case VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR: return "A1B5G5R5 UNorm Pack16";
            case VK_FORMAT_A8_UNORM_KHR: return "A8 UNorm";
            default: return "UNKNOWN_VK_FORMAT";
        }
    }

    std::string_view ShaderFormat(VkFormat format)
    {
        switch (format)
        {
            case VK_FORMAT_UNDEFINED: return "Undefined";
            case VK_FORMAT_R32_SINT: return "int";
            case VK_FORMAT_R32_SFLOAT: return "float";
            case VK_FORMAT_R32G32_SFLOAT: return "vec2";
            case VK_FORMAT_R32G32B32_SFLOAT: return "vec3";
            case VK_FORMAT_R32G32B32A32_SFLOAT: return "vec4";
            case VK_FORMAT_R64_SINT: return "long";
            case VK_FORMAT_R64_SFLOAT: return "double";
            default: return Format(format);
        }
    }

    std::string_view ColorSpace(VkColorSpaceKHR colorSpace)
    {
        switch (colorSpace)
        {
            case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: return "SRGB Nonlinear";
            case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT: return "Display P3 Nonlinear";
            case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT: return "Extended SRGB Linear";
            case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT: return "Display P3 Linear";
            case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT: return "DCI P3 Nonlinear";
            case VK_COLOR_SPACE_BT709_LINEAR_EXT: return "BT709 Linear";
            case VK_COLOR_SPACE_BT709_NONLINEAR_EXT: return "BT709 Nonlinear";
            case VK_COLOR_SPACE_BT2020_LINEAR_EXT: return "BT2020 Linear";
            case VK_COLOR_SPACE_HDR10_ST2084_EXT: return "HDR10 ST2084";
            case VK_COLOR_SPACE_DOLBYVISION_EXT: return "DolbyVision";
            case VK_COLOR_SPACE_HDR10_HLG_EXT: return "HDR10 HLG";
            case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT: return "AdobeRGB Linear";
            case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT: return "AdobeRGB Nonlinear";
            case VK_COLOR_SPACE_PASS_THROUGH_EXT: return "Pass Through";
            case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT: return "Extended SRGB Nonlinear";
            case VK_COLOR_SPACE_DISPLAY_NATIVE_AMD: return "Display Native AMD";
            default: return "UNKNOWN_COLOR_SPACE";
        }
    }

    std::string_view PresentMode(VkPresentModeKHR presentMode)
    {
        switch (presentMode)
        {
            case VK_PRESENT_MODE_IMMEDIATE_KHR: return "Immediate";
            case VK_PRESENT_MODE_MAILBOX_KHR: return "Mailbox";
            case VK_PRESENT_MODE_FIFO_KHR: return "FIFO";
            case VK_PRESENT_MODE_FIFO_RELAXED_KHR: return "FIFO Relaxed";
            case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR: return "Shared Demand Refresh";
            case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR: return "Shared Continuous Refresh";
            default: return "Unknown Present Mode";
        }
    }

    std::string_view PhysicalDeviceType(VkPhysicalDeviceType physicalDeviceType)
    {
        switch (physicalDeviceType)
        {
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "Discrete GPU";
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "Virtual GPU";
            case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
            case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "Other";
            default: return "Unknown Device Type";
        }
    }

    std::string Version(uint32_t version)
    {
        return fmt::format("{}.{}.{}", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
    }
}

