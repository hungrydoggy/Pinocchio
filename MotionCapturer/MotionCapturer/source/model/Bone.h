#pragma once

#include <string>

#include "../util/defines.h"
#include "../util/Matrix.h"


using namespace std;

class BoneData;


class Bone
{
public:
    enum BoneType
    {
        None,
        Hip_Center,
        Spine,
        Shoulder_Center,
            
        Head,
            
        Shoulder_Right,
        Elbow_Right,
        Wrist_Right,
        Hand_Right,


        Shoulder_Left,
        Elbow_Left,
        Wrist_Left,
        Hand_Left,

        Hip_Right,
        Knee_Right,
        Ankle_Right,
        Foot_Right,

        Hip_Left,
        Knee_Left,
        Ankle_Left,
        Foot_Left
    };


	// ��� ����
private:
    Bone *parent;
    string name;
    BoneType type;
    Matrix transform;
    Matrix transformFromParent;  // �θ�� ������ ��ȯ���


	// ��� �Լ�
public:
    Bone(Bone *parent, const string &name, BoneType type);


    /// �� Ű�������� �� �����͸� alpha[0,1]�� ������ �������Ͽ� ��ȯ��� ������Ʈ
    void update(BoneData *curData, BoneData *nextData, float alpha);

    void draw();
	

    // ����
	GETTER(BoneType, BoneType, type)
    GETTER_CONST_REF(string, Name ,name)
    GETTER_CONST_REF(Matrix, Transform, transform)
    GETTER_SETTER_CONST_REF(Matrix, TransformFromParent, transformFromParent)
};
