/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include "mads/mads.h"

#include "base/plugins.h"
#include "engines/advancedDetector.h"

#include "common/savefile.h"
#include "common/str-array.h"
#include "common/memstream.h"
#include "common/system.h"
#include "common/translation.h"
#include "graphics/surface.h"

#include "mads/events.h"
#include "mads/game.h"
#include "mads/detection.h"

#define MAX_SAVES 99

namespace MADS {

static const ADExtraGuiOptionsMap optionsList[] = {
	{
		GAMEOPTION_EASY_MOUSE,
		{
			_s("Easy mouse interface"),
			_s("Shows object names when hovering the mouse over them"),
			"EasyMouse",
			true,
			0,
			0
		}
	},

	{
		GAMEOPTION_ANIMATED_INVENTORY,
		{
			_s("Animated inventory items"),
			_s("Animated inventory items"),
			"InvObjectsAnimated",
			true,
			0,
			0
		}
	},

	{
		GAMEOPTION_ANIMATED_INTERFACE,
		{
			_s("Animated game interface"),
			_s("Animated game interface"),
			"TextWindowAnimated",
			true,
			0,
			0
		}
	},

	{
		GAMEOPTION_NAUGHTY_MODE,
		{
			_s("Naughty game mode"),
			_s("Naughty game mode"),
			"NaughtyMode",
			true,
			0,
			0
		}
	},

	/*{
		GAMEOPTION_GRAPHICS_DITHERING,
		{
			_s("Graphics dithering"),
			_s("Graphics dithering"),
			"GraphicsDithering",
			true,
			0,
			0
		}
	},*/

#ifdef USE_TTS
	{
		GAMEOPTION_TTS_NARRATOR,
		{
			_s("TTS Narrator"),
			_s("Use TTS to read the descriptions (if TTS is available)"),
			"tts_narrator",
			false,
			0,
			0
		}
	},
#endif

	AD_EXTRA_GUI_OPTIONS_TERMINATOR
};

uint32 MADSEngine::getGameID() const {
	return _gameDescription->gameID;
}

uint32 MADSEngine::getGameFeatures() const {
	return _gameDescription->features;
}

uint32 MADSEngine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::Language MADSEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

Common::Platform MADSEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

} // End of namespace MADS

class MADSMetaEngine : public AdvancedMetaEngine {
public:
	const char *getName() const override {
		return "mads";
	}

	const ADExtraGuiOptionsMap *getAdvancedExtraGuiOptions() const override {
		return MADS::optionsList;
	}

	bool hasFeature(MetaEngineFeature f) const override;
	Common::Error createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const override;

	SaveStateList listSaves(const char *target) const override;
	int getMaximumSaveSlot() const override;
	void removeSaveState(const char *target, int slot) const override;
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const override;
};

bool MADSMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
	    (f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportThumbnail) ||
		(f == kSimpleSavesNames);
}

bool MADS::MADSEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsReturnToLauncher) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

Common::Error MADSMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	*engine = new MADS::MADSEngine(syst, (const MADS::MADSGameDescription *)desc);
	return Common::kNoError;
}

SaveStateList MADSMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String saveDesc;
	Common::String pattern = Common::String::format("%s.0##", target);
	MADS::MADSSavegameHeader header;

	filenames = saveFileMan->listSavefiles(pattern);

	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		const char *ext = strrchr(file->c_str(), '.');
		int slot = ext ? atoi(ext + 1) : -1;

		if (slot >= 0 && slot < MAX_SAVES) {
			Common::InSaveFile *in = g_system->getSavefileManager()->openForLoading(*file);

			if (in) {
				if (MADS::Game::readSavegameHeader(in, header))
					saveList.push_back(SaveStateDescriptor(this, slot, header._saveName));
				delete in;
			}
		}
	}

	// Sort saves based on slot number.
	Common::sort(saveList.begin(), saveList.end(), SaveStateDescriptorSlotComparator());
	return saveList;
}

int MADSMetaEngine::getMaximumSaveSlot() const {
	return MAX_SAVES;
}

void MADSMetaEngine::removeSaveState(const char *target, int slot) const {
	Common::String filename = Common::String::format("%s.%03d", target, slot);
	g_system->getSavefileManager()->removeSavefile(filename);
}

SaveStateDescriptor MADSMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	Common::String filename = Common::String::format("%s.%03d", target, slot);
	Common::InSaveFile *f = g_system->getSavefileManager()->openForLoading(filename);

	if (f) {
		MADS::MADSSavegameHeader header;
		if (!MADS::Game::readSavegameHeader(f, header, false)) {
			delete f;
			return SaveStateDescriptor();
		}
		delete f;

		// Create the return descriptor
		SaveStateDescriptor desc(this, slot, header._saveName);
		desc.setThumbnail(header._thumbnail);
		desc.setSaveDate(header._year, header._month, header._day);
		desc.setSaveTime(header._hour, header._minute);
		desc.setPlayTime(header._totalFrames * GAME_FRAME_TIME);

		return desc;
	}

	return SaveStateDescriptor();
}

#if PLUGIN_ENABLED_DYNAMIC(MADS)
	REGISTER_PLUGIN_DYNAMIC(MADS, PLUGIN_TYPE_ENGINE, MADSMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(MADS, PLUGIN_TYPE_ENGINE, MADSMetaEngine);
#endif
