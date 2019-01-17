//======================
// Sample LS file
//======================

var { Alpha, Beta, Gamma, Line }

axiom
{
	Alpha(1 - 2) 
	Beta(3.3 * 2.0 + 1.0, 23.1) 
	Alpha(32 / 4)
}

p0a:
	Alpha(int x) : 0.5 ->
		[ Alpha(x * 2 + 3 * x * x + (x >> 2) ) ]
		Beta(x, x - 1) 
		Alpha(x * x + (x >> 2) )
	;

p0b:
	Alpha(int x) : 0.5 ->
		Alpha(2 * x) 
		Beta(-x % -33, x + 1) 
	;

p2:
	Beta(float x, float y) ->
		Alpha((x + 1.0) - y / 12.0)
	;

p3: 
	Beta(float x, float y) < Alpha(float z) > Beta(float v, float w) -> 
		Line(x + 10.0, y - 100.0, z * 220.0, z / w)
	;

p4: 
	Beta(int x, int y) ->
		Beta(x + 1, x * 2)
	;
