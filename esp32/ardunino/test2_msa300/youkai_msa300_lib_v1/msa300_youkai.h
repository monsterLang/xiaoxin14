typedef void (*mpythonGestureHandlePtr)(void);
class MSA300{
public:
    enum Gesture {
        /**
         * Raised when shaken
         */
        Shake = 0,
        /**
         * Raised when the screen is pointing down and the board is horizontal
         */
        ScreenUp = 1,
        /**
         * Raised when the screen is pointing up and the board is horizontal
         */
        ScreenDown = 2,
        /**
         * Raised when the screen is pointing left
         */
        TiltLeft = 3,
        /**
         * Raised when the screen is pointing right
         */
        TiltRight = 4,
        /**
         * Raised when the screen is pointing left
         */
        TiltForward = 5,
        /**
         * Raised when the screen is pointing right
         */
        TiltBack = 6,
        GestureNone = 7
    };

    MSA300();

    void init(void);
    float getX(void);
    float getY(void);
    float getZ(void);
    float getStrength(void);
    void onGesture(Gesture gesture, mpythonGestureHandlePtr body);
    bool isGesture(Gesture gesture);

private:
    float rawX, rawY, rawZ;
    mpythonGestureHandlePtr GestureHandle[7];
    Gesture currentGesture = GestureNone;
    bool isGestureEnable = false;
    bool _is_shaked = false;
    float getRawX(void);
    float getRawY(void);
    float getRawZ(void);
    static void taskLoop(void *param){
        MSA300 *self = (MSA300 *)param;
        char type = ' ';
        float _last_x, _last_y, _last_z, diff_x, diff_y, diff_z;
        int _count_shaked = 0;
        while(1){
			wait:
            delay(100);//delay放前面，让其他iic设备先行
            if(self->_is_shaked){
                _count_shaked += 1;
                if(_count_shaked==5)
                    _count_shaked = 0;
            }
            float x = self->getRawX();
            float y = self->getRawY();
            float z = self->getRawZ();
            if(_last_x == 0 && _last_y == 0 && _last_z == 0){
                _last_x = x; _last_y = y; _last_z = z;
                goto yyy;
            }
            diff_x = x - _last_x; diff_y = y - _last_y; diff_z = z - _last_z;
            _last_x = x; _last_y = y; _last_z = z;
            if(_count_shaked > 0)
                goto yyy;
            self->_is_shaked = (diff_x * diff_x + diff_y * diff_y + diff_z * diff_z) > 1;
            yyy:
            if(self->_is_shaked){
                self->currentGesture = Shake;
                if(type == '0') goto wait;
                type = '0';
                if(self->GestureHandle[Shake]) self->GestureHandle[Shake]();
                goto wait;
            }
            else if(x < -0.5){
                self->currentGesture = TiltForward;
                if(type == '5') goto wait;
                type='5';
                if(self->GestureHandle[TiltForward]) self->GestureHandle[TiltForward]();
            }else if(x > 0.5){
                self->currentGesture = TiltBack;
                if(type == '6')  goto wait;
                type='6';
                if(self->GestureHandle[TiltBack]) self->GestureHandle[TiltBack]();
            }else if(y < -0.5){
                self->currentGesture = TiltRight;
                if(type == '4')  goto wait;
                type='4';
                if(self->GestureHandle[TiltRight]) self->GestureHandle[TiltRight]();
            }else if(y > 0.5){
                self->currentGesture = TiltLeft;
                if(type == '3')  goto wait;
                type='3';
                if(self->GestureHandle[TiltLeft]) self->GestureHandle[TiltLeft]();
            }else if(fabs(x)<0.2 && fabs(y)<0.2 && z>0){
                self->currentGesture = ScreenDown;
                if(type == '2')  goto wait;
                type='2';
                if(self->GestureHandle[ScreenDown]) self->GestureHandle[ScreenDown]();
            }else if(fabs(x)<0.2 && fabs(y)<0.2 && z<0){
                self->currentGesture = ScreenUp;
                if(type == '1')  goto wait;
                type='1';
                if(self->GestureHandle[ScreenUp]) self->GestureHandle[ScreenUp]();
            }else{
                self->currentGesture = GestureNone;
                type = ' ';
            }
        }
    }
};
