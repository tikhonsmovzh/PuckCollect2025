#pragma once

class Base{ // ну и хуйню понаписал XD
    private:
    const float movementCoefficient = 0.5;
    pair<DcMotor, DcMotor> Motors;
    BNO055Gyro Gyroscop;
    Position _mainPosition = Position(); // почти все в еденицах измерения поля

    Field _mainField = Field();

    float getDistance(Position RobotPos, Position TargetPos){
        float dist = sqrt(pow(TargetPos.x - RobotPos.x, 2) + pow(TargetPos.y - RobotPos.y, 2));
        return dist;
    }

    float getGlobalError(Position errorPos){
        return (errorPos.x + errorPos.y + errorPos.z.x) / 3
    }
    public:

    Base(pair<DcMotor, DcMotor> Motors, BNO055Gyro Gyroscop){ // сюда надо засунуть еще и датчики. Я буду калиброать робота что бы кататься по кругу с их помощью
        ResetPosition();
        Motors = Motors;
        Gyroscop = Gyroscop;
    }

    void ResetPosition(){
        _mainPosition.pos(0f, 0f, Gyroscop.getOrientation());
    }

    /* определение глобальных координат
                  |
                  | ^
                **| |
    --------------- x (+)
               <--y
                 (+)
    */

    struct Position(float x, float y){ // да тих, я просто скопировал твой код. Все норм XD
        float x, y; // еденицы измерения поля это еденица енкодера ***!
        Oriantation z;
        pos(float x, float y, Oriantation z){
            this->x = x;
            this->y = y;
            this->z = z;
        }

    }

    struct Field(){
        Position leftUp, leftDown, rightUp, rightDown;
        letfUp = Position(1, 0); // потом свапним на замеры
        leftDown = Position(0, 0);
        rightUp = Position(1, 1);
        rightDown = Psition(0, 1);
    }
    
    // радиус будет в еденицах измеренния поля
    void circleDrive(float Radius){ // это будет запускаться с центра стены после калибровки
        const Position StartPosition = _mainPosition;
        const Position CircleCenter = Position(_mainField.rightDown.y / 2, _mainField.leftUp.x / 2);
        
    }

    void GoToTarget(Position targetPosition){
        Position errorPosition = Position();
        errorPosition.pos(
            targetPosition.x - _mainPosition.x, 
            targetPosition.y - _mainPosition.y, 
            targetPosition.z.x - _mainPosition.z.x
        );
        while(getGlobalerror(errorPosition)){

        }
    }
}