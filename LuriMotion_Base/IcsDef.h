#pragma once

enum class OperationMode
{
	Auto,
	Manual,
	SemiAuto,
	StartUpCheck,
	DryRun,
	Loto,
};

enum class InspectionPortType
{
	Left,		
	Right
};

enum class PortType
{
	RFID,
	Buffer,
	Left,
	Right,
	All
};

enum class ConveyorType
{
	RFID,
	Inspection,
	Returning
};

enum class EquipmentStatus
{
	None,  // 초기화 이전
	Init,  // 원점 수행중
	Idle,  // 대기
	Run,   // 동작중
	Alarm  // 알람
};

enum class PortStatus
{
	Empty,
	Loaded,
	Unloading,
	Disabled,
	Alarm
};

enum class ConveyorStatus1
{
	Idle,
	Moving,
	Alarm
};

enum class ConveyorStatus2
{
	Empty,
	Loaded
};
