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
	None,  // �ʱ�ȭ ����
	Init,  // ���� ������
	Idle,  // ���
	Run,   // ������
	Alarm  // �˶�
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
