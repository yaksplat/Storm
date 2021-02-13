class LinearFader
{
  public:
    void begin(void)
    {
      _totalSteps = 1000;
      reset();
      _ended = true;
      Serial.println("Begin");
    }

    inline void reset(void)
    {
      Serial.println("reset");
      setFade(_rStart, _gStart, _bStart, _wStart, _rEnd, _gEnd, _bEnd, _wEnd);
    }

    inline bool isEnded(void) {
      return (_ended);
    }

    inline void setSteps(int p) {
      Serial.println("set steps");
      
      _totalSteps = p;    
    }

    void setFade(const uint32_t color1, const uint32_t color2) {
Serial.println("set fade");
      byte white1 = (color1 >> 24) & 0xff;
      byte red1 = (color1 >> 16) & 0xff;
      byte green1 = (color1 >> 8) & 0xff;
      byte blue1 = color1 & 0xff;

      byte white2 = (color2 >> 24) & 0xff;
      byte red2 = (color2 >> 16) & 0xff;
      byte green2 = (color2 >> 8) & 0xff;
      byte blue2 = color2 & 0xff;

      setFade(red1, green1, blue1, white1, red2, green2, blue2, white2);
    }

    void setFade(uint16_t r0, uint16_t g0, uint16_t b0, uint16_t w0, uint16_t r1, uint16_t g1, uint16_t b1, uint16_t w1)
    {
      Serial.println("set fade 2");
      Serial.print("r0:");
      Serial.println(r0);
      Serial.print("g0:");
      Serial.println(g0);
      Serial.print("b0:");
      Serial.println(b0);
      Serial.print("w0:");
      Serial.println(w0);

      Serial.print("r1:");
      Serial.println(r1);
      Serial.print("g1:");
      Serial.println(g1);
      Serial.print("b1:");
      Serial.println(b1);
      Serial.print("w1:");
      Serial.println(w1);

      
      _rStart = _r0 = r0;  _rEnd = _r1 = r1;
      _gStart = _g0 = g0;  _gEnd = _g1 = g1;
      _bStart = _b0 = b0;  _bEnd = _b1 = b1;
      _wStart = _w0 = w0;  _wEnd = _w1 = w1;

      _dR = uiabs(_r1, _r0);  
      _dG = uiabs(_g1, _g0);  
      _dB = uiabs(_b1, _b0); 
      _dW = uiabs(_w1, _w0); 

      _sR = _r0 < _r1 ? 1 : -1;
      _sG = _g0 < _g1 ? 1 : -1;
      _sB = _b0 < _b1 ? 1 : -1;
      _sW = _w0 < _w1 ? 1 : -1;
      
      Serial.print("_dR:");
      Serial.println(_dR);
      Serial.print("_dG:");
      Serial.println(_dG);
      Serial.print("_dB:");
      Serial.println(_dB);
      Serial.print("_dW:");
      Serial.println(_dW);
      

      _dMax = max4(_dR, _dG, _dB, _dW);
      Serial.print("_dMax:");
      Serial.println(_dMax);
     // _steps = 147; //_dMax
     // val = map(val, 0, 1023, 0, 255);
     // _sR = map(_currentStep, 0 ,steps ,r0 ,r1 );
     _currentStep = _totalSteps;
     // _r1 = _g1 = _b1 = _dMax / 2;          /* error offset */

      _ended = false;
    }

    bool getNext(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &w)
    {
      Serial.println("GetNext");
      r = _r0;
      g = _g0;
      b = _b0;
      w = _w0;
      
      Serial.print("currentStep:");
      Serial.println(_currentStep);
      _ended = (_currentStep == 0);
      if (!_ended)
      {
        _currentStep--;

        _r0 = map(_currentStep, 0 ,_totalSteps ,_r0 ,_r1 );
        _g0 = map(_currentStep, 0 ,_totalSteps ,_g0 ,_g1 );
        _b0 = map(_currentStep, 0 ,_totalSteps ,_b0 ,_b1 );
        _w0 = map(_currentStep, 0 ,_totalSteps ,_w0 ,_w1 );
        
     //   _r1 -= _dR; if (_r1 < 0) {
     //     _r1 += _dMax;
      //    _r0 += _sR;
     //   }
      //  _g1 -= _dG; if (_g1 < 0) {
     //     _g1 += _dMax;
     //     _g0 += _sG;
     //   }
     //   _b1 -= _dB; if (_b1 < 0) {
     //     _b1 += _dMax;
     //     _b0 += _sB;
     //   }
     //   _w1 -= _dW; if (_w1 < 0) {
     //     _w1 += _dMax;
     //     _w0 += _sW;
     //   }
      }
      return (true);
    }

  private:
    uint16_t _rStart, _gStart, _bStart, _wStart, _rEnd, _gEnd, _bEnd, _wEnd;
    int32_t _r0, _g0, _b0, _w0, _r1, _g1, _b1, _w1, _totalSteps, _currentStep;
    int16_t _dR, _dG, _dB, _dW, _dMax;
    int8_t _sR, _sG, _sB, _sW;
    bool _ended;

    uint16_t max3(uint16_t x, uint16_t y, uint16_t z)
    {
      uint16_t m = 0; 
      if(x > m) m=x;
      if(y > m) m=y; 
      if(z > m) m=z;     
      return (m);
    }

    uint16_t max4(uint16_t w, uint16_t x, uint16_t y, uint16_t z)
    {
      uint16_t m = 0; 
      if(w > m) m=w;
      if(x > m) m=x;
      if(y > m) m=y; 
      if(z > m) m=z;     
      return (m);
    }

    //absolute value between a and b
    inline uint16_t uiabs(uint16_t a, uint16_t b) {
      return ((a > b) ? (a - b) : (b - a));
    }

};
