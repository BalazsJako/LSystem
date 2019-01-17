//======================
// Sample LS file
//======================

var
	Alpha
	Beta
	Gamma
	Delta

//state
//	int a;
//	vec3 b;

axiom:
	Alpha(1.0)
	Alpha(1 - 2) 
	Beta(3.3 * 2.0 + 1.0, 23.1) 
	Alpha(32 / 4)
	Gamma( vec2(0.0, 1.0) )
	Gamma( vec3(0.0, 0.0, 1.0) )
	Gamma( vec4(0.0, 0.0, 0.0, 1.0) );

p0a:
	Alpha(int x) when x > 1 weight 0.5 
	{
		Alpha( x * 2 + 3 * x * x + (x >> 2) )
		Beta(x, x - 1) 
		Alpha(x * x + (x >> 2) )
	}

p0b:
	Alpha(int x) when x == 2 weight 0.5
	{
		Alpha((int)(2.4 * (float)(x - 1)))
		Beta(-x % -33, x + (int)(1.0)) 
	}

functest:
	Alpha(float x) ? x > 1.0 ->
		Alpha(cos(x) * cos(x) + sin(x) * abs(max(sin(x), ceil(log(x))) - atan2(x, -x) * log10(sqrt(mod(x, 10.0))) + min(floor(exp(x)), round(pow(x, 3.0)))))
	;

p2:
	Beta(int x, int y) ->
		Alpha((x + 1) - y / 12)
	;

//p3: 
//	Beta(float x, float y) < Alpha(float z) > Beta(float v, float w) -> 
//		Line(x + 10.0, y - 100.0, z * 220.0, z / w)
//	;

p4: 
	Alpha(int x) Alpha(int x2) 
			< Beta(int y, int z) > 
	Alpha(int w) 
	->
		Beta(x + 1 + z, y * 2 - z + w) 
	;

vectest2:
	Gamma(vec2 v) ->
		Gamma(v + vec2(1.0, 2.0) + v.yx + dot(v, vec2(1.0, 0.0)))
	;

vectest3:
	Gamma(vec3 v) ->
		Gamma(v + vec3(1.0, 1.0 + 4.0, 2.0 + dot(v, vec3(0.0, 1.0, 0.0))))
	;

vectest4:
	Gamma(vec4 v) ->
		Gamma(v + vec4(0.4 + v.z, 1.0 + v.w, 1.0 + v.y, 2.0 + v.x) * dot(v.xyzw, vec4(0.0, 1.0, 2.0, 3.0)))
	;
