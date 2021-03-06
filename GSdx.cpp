/*
 *	Copyright (C) 2007-2009 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "stdafx.h"
#include "GSdx.h"

static void* s_hModule;

#ifdef _WINDOWS

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		s_hModule = hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

bool GSdxApp::LoadResource(int id, vector<unsigned char>& buff, const char* type)
{
	buff.clear();
	HRSRC hRsrc = FindResource((HMODULE)s_hModule, MAKEINTRESOURCE(id), type != NULL ? type : RT_RCDATA);
	if(!hRsrc) return false;
	HGLOBAL hGlobal = ::LoadResource((HMODULE)s_hModule, hRsrc);
	if(!hGlobal) return false;
	DWORD size = SizeofResource((HMODULE)s_hModule, hRsrc);
	if(!size) return false;
	buff.resize(size);
	memcpy(buff.data(), LockResource(hGlobal), size);
	return true;
}

#else

bool GSdxApp::LoadResource(int id, vector<unsigned char>& buff, const char* type)
{
	buff.clear();
	printf("LoadResource not implemented\n");
	return false;
}

size_t GSdxApp::GetPrivateProfileString(const char* lpAppName, const char* lpKeyName, const char* lpDefault, char* lpReturnedString, size_t nSize, const char* lpFileName)
{
	BuildConfigurationMap(lpFileName);

	std::string key(lpKeyName);
	std::string value = m_configuration_map[key];
	if (value.empty()) {
		// save the value for futur call
		m_configuration_map[key] = std::string(lpDefault);
		strcpy(lpReturnedString, lpDefault);
	} else
		strcpy(lpReturnedString, value.c_str());

    return 0;
}

bool GSdxApp::WritePrivateProfileString(const char* lpAppName, const char* lpKeyName, const char* pString, const char* lpFileName)
{
	BuildConfigurationMap(lpFileName);

	std::string key(lpKeyName);
	std::string value(pString);
	m_configuration_map[key] = value;

	// Save config to a file
	FILE* f = fopen(lpFileName, "w");

	if (f == NULL) return false; // FIXME print a nice message

	map<std::string,std::string>::iterator it;
	for (it = m_configuration_map.begin(); it != m_configuration_map.end(); ++it) {
		// Do not save the inifile key which is not an option
		if (it->first.compare("inifile") == 0) continue;

		if (!it->second.empty())
			fprintf(f, "%s = %s\n", it->first.c_str(), it->second.c_str());
	}
	fclose(f);

	return false;
}

int GSdxApp::GetPrivateProfileInt(const char* lpAppName, const char* lpKeyName, int nDefault, const char* lpFileName)
{
	BuildConfigurationMap(lpFileName);

	std::string value = m_configuration_map[std::string(lpKeyName)];
	if (value.empty()) {
		// save the value for futur call
		SetConfig(lpKeyName, nDefault);
		return nDefault;
	} else
		return atoi(value.c_str());
}
#endif

GSdxApp theApp;

GSdxApp::GSdxApp()
{
	m_ini = "inis/GSdx-Cutie.ini";
	m_section = "Settings";

	m_gs_renderers.push_back(GSSetting(0, "Direct3D9", "Hardware"));
	m_gs_renderers.push_back(GSSetting(1, "Direct3D9", "Software"));
	m_gs_renderers.push_back(GSSetting(14, "Direct3D9", "OpenCL"));
	m_gs_renderers.push_back(GSSetting(2, "Direct3D9", "Null"));
	m_gs_renderers.push_back(GSSetting(3, "Direct3D", "Hardware"));
	m_gs_renderers.push_back(GSSetting(4, "Direct3D", "Software"));
	m_gs_renderers.push_back(GSSetting(15, "Direct3D", "OpenCL"));
	m_gs_renderers.push_back(GSSetting(5, "Direct3D", "Null"));
	m_gs_renderers.push_back(GSSetting(10, "Null", "Software"));
	m_gs_renderers.push_back(GSSetting(16, "Null", "OpenCL"));
	m_gs_renderers.push_back(GSSetting(11, "Null", "Null"));
	m_gs_renderers.push_back(GSSetting(12, "OpenGL", "Hardware"));
	m_gs_renderers.push_back(GSSetting(13, "OpenGL", "Software"));
	m_gs_renderers.push_back(GSSetting(17, "OpenGL", "OpenCL"));

	m_gs_interlace.push_back(GSSetting(0, "None", ""));
	m_gs_interlace.push_back(GSSetting(1, "Weave tff", "saw-tooth"));
	m_gs_interlace.push_back(GSSetting(2, "Weave bff", "saw-tooth"));
	m_gs_interlace.push_back(GSSetting(3, "Bob tff", "use blend if shaking"));
	m_gs_interlace.push_back(GSSetting(4, "Bob bff", "use blend if shaking"));
	m_gs_interlace.push_back(GSSetting(5, "Blend tff", "slight blur, 1/2 fps"));
	m_gs_interlace.push_back(GSSetting(6, "Blend bff", "slight blur, 1/2 fps"));
	m_gs_interlace.push_back(GSSetting(7, "Auto", ""));

	m_gs_aspectratio.push_back(GSSetting(0, "Stretch", ""));
	m_gs_aspectratio.push_back(GSSetting(1, "4:3", ""));
	m_gs_aspectratio.push_back(GSSetting(2, "16:9", ""));

	m_gs_upscale_multiplier.push_back(GSSetting(1, "Custom", ""));
	m_gs_upscale_multiplier.push_back(GSSetting(2, "2x Native", ""));
	m_gs_upscale_multiplier.push_back(GSSetting(3, "3x Native", ""));
	m_gs_upscale_multiplier.push_back(GSSetting(4, "4x Native", ""));
	m_gs_upscale_multiplier.push_back(GSSetting(5, "5x Native", ""));
	m_gs_upscale_multiplier.push_back(GSSetting(6, "6x Native", ""));

	m_gs_HDmode.push_back(GSSetting(1, "Disable", ""));
	m_gs_HDmode.push_back(GSSetting(2, "mode 1", ""));
	m_gs_HDmode.push_back(GSSetting(3, "mode 1-1", ""));
	m_gs_HDmode.push_back(GSSetting(4, "mode 1-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(5, "mode 1-2", ""));
	m_gs_HDmode.push_back(GSSetting(6, "mode 1-3", ""));
	m_gs_HDmode.push_back(GSSetting(7, "mode 2", ""));
	m_gs_HDmode.push_back(GSSetting(8, "mode 2-1", ""));
	m_gs_HDmode.push_back(GSSetting(9, "mode 2-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(10, "mode 2-2", ""));
	m_gs_HDmode.push_back(GSSetting(11, "mode 2-3", ""));
	m_gs_HDmode.push_back(GSSetting(12, "mode 3", ""));
	m_gs_HDmode.push_back(GSSetting(13, "mode 3-1", ""));
	m_gs_HDmode.push_back(GSSetting(14, "mode 3-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(15, "mode 3-2", ""));
	m_gs_HDmode.push_back(GSSetting(16, "mode 3-3", ""));
	m_gs_HDmode.push_back(GSSetting(17, "mode 4", ""));
	m_gs_HDmode.push_back(GSSetting(18, "mode 4-1", ""));
	m_gs_HDmode.push_back(GSSetting(19, "mode 4-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(20, "mode 4-2", ""));
	m_gs_HDmode.push_back(GSSetting(21, "mode 4-3", ""));
	m_gs_HDmode.push_back(GSSetting(22, "mode 5", ""));
	m_gs_HDmode.push_back(GSSetting(23, "mode 5-1", ""));
	m_gs_HDmode.push_back(GSSetting(24, "mode 5-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(25, "mode 5-1-2", ""));
	m_gs_HDmode.push_back(GSSetting(26, "mode 5-2", ""));
	m_gs_HDmode.push_back(GSSetting(27, "mode 5-3", ""));
	m_gs_HDmode.push_back(GSSetting(28, "mode 6", ""));
	m_gs_HDmode.push_back(GSSetting(29, "mode 6-1", ""));
	m_gs_HDmode.push_back(GSSetting(30, "mode 6-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(31, "mode 6-2", ""));
	m_gs_HDmode.push_back(GSSetting(32, "mode 6-2-1", ""));
	m_gs_HDmode.push_back(GSSetting(33, "mode 6-3", ""));
	m_gs_HDmode.push_back(GSSetting(34, "mode 7", ""));
	m_gs_HDmode.push_back(GSSetting(35, "mode 7-1", ""));
	m_gs_HDmode.push_back(GSSetting(36, "mode 7-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(37, "mode 7-2", ""));
	m_gs_HDmode.push_back(GSSetting(38, "mode 7-3", ""));
	m_gs_HDmode.push_back(GSSetting(39, "mode 8", ""));
	m_gs_HDmode.push_back(GSSetting(40, "mode 8-1", ""));
	m_gs_HDmode.push_back(GSSetting(41, "mode 8-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(42, "mode 8-2", ""));
	m_gs_HDmode.push_back(GSSetting(43, "mode 8-3", ""));
	m_gs_HDmode.push_back(GSSetting(44, "mode 9", ""));
	m_gs_HDmode.push_back(GSSetting(45, "mode 9-1", ""));
	m_gs_HDmode.push_back(GSSetting(46, "mode 9-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(47, "mode 9-2", ""));
	m_gs_HDmode.push_back(GSSetting(48, "mode 9-3", ""));
	m_gs_HDmode.push_back(GSSetting(49, "mode 10", ""));
	m_gs_HDmode.push_back(GSSetting(50, "mode 10-1", ""));
	m_gs_HDmode.push_back(GSSetting(51, "mode 10-1-1", ""));
	m_gs_HDmode.push_back(GSSetting(52, "mode 10-1-2", ""));
	m_gs_HDmode.push_back(GSSetting(53, "mode 10-2", ""));
	m_gs_HDmode.push_back(GSSetting(54, "mode 10-3", ""));
	m_gs_HDmode.push_back(GSSetting(57, "mode 10-4", ""));

	m_gs_max_anisotropy.push_back(GSSetting(1, "1x", "Off"));
	m_gs_max_anisotropy.push_back(GSSetting(2, "2x", ""));
	m_gs_max_anisotropy.push_back(GSSetting(4, "4x", ""));
	m_gs_max_anisotropy.push_back(GSSetting(8, "8x", ""));
	m_gs_max_anisotropy.push_back(GSSetting(16, "16x", ""));

	m_gs_filter.push_back(GSSetting(0, "Nearest", ""));
	m_gs_filter.push_back(GSSetting(1, "Bilinear", "Forced"));
	m_gs_filter.push_back(GSSetting(2, "Bilinear", "PS2"));

	m_gs_gl_ext.push_back(GSSetting(-1, "Auto", ""));
	m_gs_gl_ext.push_back(GSSetting(0,  "Force-Disabled", ""));
	m_gs_gl_ext.push_back(GSSetting(1,  "Force-Enabled", ""));

	m_gs_hack.push_back(GSSetting(0,  "Off", ""));
	m_gs_hack.push_back(GSSetting(1,  "Halfly On", ""));
	m_gs_hack.push_back(GSSetting(2,  "Fully On", ""));

	m_gpu_renderers.push_back(GSSetting(0, "Direct3D9 (Software)", ""));
	m_gpu_renderers.push_back(GSSetting(1, "Direct3D11 (Software)", ""));
	m_gpu_renderers.push_back(GSSetting(2, "SDL 1.3 (Software)", ""));
	m_gpu_renderers.push_back(GSSetting(3, "Null (Software)", ""));
	//m_gpu_renderers.push_back(GSSetting(4, "Null (Null)", ""));

	m_gpu_filter.push_back(GSSetting(0, "Nearest", ""));
	m_gpu_filter.push_back(GSSetting(1, "Bilinear (polygons only)", ""));
	m_gpu_filter.push_back(GSSetting(2, "Bilinear", ""));

	m_gpu_dithering.push_back(GSSetting(0, "Disabled", ""));
	m_gpu_dithering.push_back(GSSetting(1, "Auto", ""));

	m_gpu_aspectratio.push_back(GSSetting(0, "Stretch", ""));
	m_gpu_aspectratio.push_back(GSSetting(1, "4:3", ""));
	m_gpu_aspectratio.push_back(GSSetting(2, "16:9", ""));

	m_gpu_scale.push_back(GSSetting(0 | (0 << 2), "H x 1 - V x 1", ""));
	m_gpu_scale.push_back(GSSetting(1 | (0 << 2), "H x 2 - V x 1", ""));
	m_gpu_scale.push_back(GSSetting(0 | (1 << 2), "H x 1 - V x 2", ""));
	m_gpu_scale.push_back(GSSetting(1 | (1 << 2), "H x 2 - V x 2", ""));
	m_gpu_scale.push_back(GSSetting(2 | (1 << 2), "H x 4 - V x 2", ""));
	m_gpu_scale.push_back(GSSetting(1 | (2 << 2), "H x 2 - V x 4", ""));
	m_gpu_scale.push_back(GSSetting(2 | (2 << 2), "H x 4 - V x 4", ""));
}

#ifdef __linux__
void GSdxApp::ReloadConfig()
{
	if (m_configuration_map.empty()) return;

	auto file = m_configuration_map.find("inifile");
	if (file == m_configuration_map.end()) return;

	// A map was built so reload it
	std::string filename = file->second;
	m_configuration_map.clear();
	BuildConfigurationMap(filename.c_str());
}

void GSdxApp::BuildConfigurationMap(const char* lpFileName)
{
	// Check if the map was already built
	std::string inifile_value(lpFileName);
	if ( inifile_value.compare(m_configuration_map["inifile"]) == 0 ) return;
	m_configuration_map["inifile"] = inifile_value;

	// Load config from file
	char value[255];
	char key[255];
	FILE* f = fopen(lpFileName, "r");

	if (f == NULL) return; // FIXME print a nice message

	while( fscanf(f, "%s = %s\n", key, value) != EOF ) {
		std::string key_s(key);
		std::string value_s(value);
		m_configuration_map[key_s] = value_s;
	}

	fclose(f);
}
#endif

void* GSdxApp::GetModuleHandlePtr()
{
	return s_hModule;
}

void GSdxApp::SetConfigDir(const char* dir)
{
	if( dir == NULL )
	{
		m_ini = "inis/GSdx-Cutie.ini";
	}
	else
	{
		m_ini = dir;

		if(m_ini[m_ini.length() - 1] != DIRECTORY_SEPARATOR)
		{
			m_ini += DIRECTORY_SEPARATOR;
		}

		m_ini += "GSdx-Cutie.ini";
	}
}

string GSdxApp::GetConfig(const char* entry, const char* value)
{
	char buff[4096] = {0};

	GetPrivateProfileString(m_section.c_str(), entry, value, buff, countof(buff), m_ini.c_str());

	return string(buff);
}

void GSdxApp::SetConfig(const char* entry, const char* value)
{
	WritePrivateProfileString(m_section.c_str(), entry, value, m_ini.c_str());
}

int GSdxApp::GetConfig(const char* entry, int value)
{
	return GetPrivateProfileInt(m_section.c_str(), entry, value, m_ini.c_str());
}

void GSdxApp::SetConfig(const char* entry, int value)
{
	char buff[32] = {0};

	sprintf(buff, "%d", value);

	SetConfig(entry, buff);
}
