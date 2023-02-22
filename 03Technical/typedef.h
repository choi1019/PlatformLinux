#pragma once

#include <typedef.h>

enum class _ELayer_Technical {
	_eBegin = _GET_LAYER_UID(_ELayer::_eTechnical),

	_eMemoryObject,
	_eMemoryStatic,
	_eMemoryDynamic,
	_eMemoryEven,
	_ePageIndex,
	_ePageList,
	_eSlotList,
	_eSlotInfo,

	_eTimer,
	_eSocketServer,
	_eSocketClient,
	_eParser,

	_eEnd
};

#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Directory.h>