#pragma once

enum HxANodeType {
	MetaOnly,
	Geometry,
	Image,
	Unknown
};

enum class HxAMetaDataType {
	INT64,
	DOUBLE,
	NODE,
	TEXT,
	BINARY,
	META,
	COUNT,
	Unknown
};

enum class HxALayerDataType {
	UINT8,
	INT32,
	FLOAT,
	DOUBLE,
	Unknown
};