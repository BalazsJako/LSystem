// A very basic English sentence generator

var
	Sentence
	Statement
	Question
	Subject
	Verb
	AdverbList
	Adverb
	Object
	
	do
	to
	
	slowly
	quickly
	loudly
	quietly
	hastily
	nervously
	calmly
	
	run
	walk
	crawl
	jump
	go
	march
	
	the
	policeman
	burglar
	victim
	judge
	advocate
	
	I
	we
	you
	they
	here
	there
	everywhere
	at
	in
	museum
	city
	house
	office
	
	and
	
axiom:
	Sentence
	;
	
p0:
	Sentence : 0.5 ->
		Statement
	;
	
p1: 
	Sentence : 1.5 ->
		Question
		;
		
p2:
	Statement ->
		Subject
		Verb
		AdverbList
		Object
		;
		
p3:
	Question ->
		do
		Subject
		Verb
		AdverbList
		Object
		;
		
p4:
	AdverbList : 0.5 ->
		Adverb
		;
		
p4b:
	AdverbList : 0.2 ->
		Adverb
		and
		AdverbList
		;

p5a:	Adverb : 1.0 -> slowly;	
p5b:	Adverb : 1.0 -> quickly;
p5c:	Adverb : 1.0 -> loudly;	
p5d:	Adverb : 1.0 -> quietly;
p5d:	Adverb : 1.0 -> hastily;
p5d:	Adverb : 1.0 -> nervously;
p5d:	Adverb : 1.0 -> calmly;

p6a:	Verb : 1.0 -> run;
p6b:	Verb : 1.0 -> walk;
p6f:	Verb : 1.0 -> crawl;
p6f:	Verb : 1.0 -> jump;
p6f:	Verb : 1.0 -> go;
p6f:	Verb : 1.0 -> march;

p7a:	Object : 1.0 -> here;
p7b:	Object : 1.0 -> there;
p7c:	Object : 1.0 -> everywhere;
p7c:	Object : 1.0 -> to the museum;
p7c:	Object : 1.0 -> to the house;
p7c:	Object : 1.0 -> to the city;
p7c:	Object : 1.0 -> to the house;

p8a:	Subject : 1.0 -> I;
p8b:	Subject : 1.0 -> you;
p8c:	Subject : 1.0 -> we;
p8d:	Subject : 1.0 -> they;
p8e:	Subject : 1.0 -> the policeman;
p8f:	Subject : 1.0 -> the burglar;
