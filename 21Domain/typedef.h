#pragma once

#include <typedef.h>

enum class _ELayer_Domain {
	_eBegin = _GET_LAYER_UID(_ELayer::_eDomain),

	_eVideoManager,
	_eVideoRequesterManager,
	_eVideoProviderManager,

	_eGenerator,
	_eStreamContext,
	
	_eEnd
};



