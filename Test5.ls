var
	Initial
		
axiom:
	Initial(0)
	;
	
Prob0:
	Initial(int i) : 1.0 ->
		Initial(1)
		;
	
Prob1:
	Initial(int i) : 1.0 ->
		Initial(2)
		;
	
Prob2:
	Initial(int i) : 1.0 ->
		Initial(3)
		;
