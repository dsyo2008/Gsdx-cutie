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

#pragma once

#include "GSRenderer.h"
#include "GSTextureCache.h"
#include "GSCrc.h"
#include "GSFunctionMap.h"

class GSRendererHW : public GSRenderer
{
private:
	int m_width;
	int m_height;
	int m_skip;
	int m_UserHacks_SkipPostProcessing;
	bool m_reset;
	int m_upscale_multiplier;
	int m_userhacks_skipdraw;
	int m_HDmode;
	int m_customhdrev;
	int m_scalex;
	int m_scaley;
	
	bool m_userhacks_align_sprite_X;
	
	#pragma region hacks

	typedef bool (GSRendererHW::*OI_Ptr)(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	typedef void (GSRendererHW::*OO_Ptr)();
	typedef bool (GSRendererHW::*CU_Ptr)();

	bool OI_DevilMayCry3(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_SakigakeOtokojuku(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_ElementalGerad(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_ShadowHearts2(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Ringoftheking3(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_FFXII(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_FFX(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_MetalSlug6(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_GodOfWar(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_GodOfWar2(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_SimpsonsGame(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_RozenMaidenGebetGarden(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Siren(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_HauntingGround(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_SpidermanWoS(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_TyTasmanianTiger(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_DigimonRumbleArena2(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_BlackHawkDown(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_StarWarsForceUnleashed(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_XmenOriginsWolverine(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_CallofDutyFinalFronts(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_SpyroNewBeginning(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_SpyroEternalNight(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_TalesOfLegendia(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_SMTNocturne(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_PoisonPink(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Simple2000Vol15(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Mercenaries2(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Kof98um(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Simple2000Vol55(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Simple2000Vol30(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_FD18(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Onimusha2(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Okami(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_SilentHillOrigins(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_FFVIIDoC(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_JusticeLeagueHeros(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Growlanser6(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_Black(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	bool OI_PointListPalette(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* t);
	void OO_DBZBT2();
	void OO_MajokkoALaMode2();

	bool CU_DBZBT2();
	bool CU_MajokkoALaMode2();
	bool CU_TalesOfAbyss();

	class Hacks
	{
		template<class T> class HackEntry
		{
		public:
			CRC::Title title;
			CRC::Region region;
			T func;

			HackEntry(CRC::Title t, CRC::Region r, T f)
			{
				title = t;
				region = r;
				func = f;
			}
		};

		template<class T> class FunctionMap : public GSFunctionMap<uint32, T>
		{
			list<HackEntry<T> >& m_tbl;

			T GetDefaultFunction(uint32 key)
			{
				CRC::Title title = (CRC::Title)(key & 0xffffff);
				CRC::Region region = (CRC::Region)(key >> 24);

				for(typename list<HackEntry<T> >::iterator i = m_tbl.begin(); i != m_tbl.end(); i++)
				{
					if(i->title == title && (i->region == CRC::RegionCount || i->region == region))
					{
						return i->func;
					}
				}

				return NULL;
			}

		public:
			FunctionMap(list<HackEntry<T> >& tbl) : m_tbl(tbl) {}
		};

		list<HackEntry<OI_Ptr> > m_oi_list;
		list<HackEntry<OO_Ptr> > m_oo_list;
		list<HackEntry<CU_Ptr> > m_cu_list;

		FunctionMap<OI_Ptr> m_oi_map;
		FunctionMap<OO_Ptr> m_oo_map;
		FunctionMap<CU_Ptr> m_cu_map;

	public:
		OI_Ptr m_oi;
		OO_Ptr m_oo;
		CU_Ptr m_cu;

		Hacks();

		void SetGameCRC(const CRC::Game& game);

	} m_hacks;

	#pragma endregion

	int Interpolate_UV(float alpha, int t0, int t1);
	float alpha0(int L, int X0, int X1);
	float alpha1(int L, int X0, int X1);

	template <bool linear> void RoundSpriteOffset();

protected:
	GSTextureCache* m_tc;

	virtual void DrawPrims(GSTexture* rt, GSTexture* ds, GSTextureCache::Source* tex) = 0;

	int m_userhacks_round_sprite_offset;

public:
	GSRendererHW(GSTextureCache* tc);
	virtual ~GSRendererHW();

	void SetGameCRC(uint32 crc, int options);
	bool CanUpscale();
	int GetUpscaleMultiplier();

	void Reset();
	void VSync(int field);
	void ResetDevice();
	GSTexture* GetOutput(int i);
	void InvalidateVideoMem(const GIFRegBITBLTBUF& BITBLTBUF, const GSVector4i& r);
	void InvalidateLocalMem(const GIFRegBITBLTBUF& BITBLTBUF, const GSVector4i& r, bool clut = false);
	void Draw();
};
