program test1;
type
	tp = record
		x,y:integer;
	end;
	
var
	d: array [1..5] of integer;
	f: real;
	a, b, c: integer;
	e: tp;
begin
	c:=2;
	f:=3.4;
	e.y:=0;
	d[3]:=0;
	
	while d[4]>0 do begin
		c:=c+1+2*3;
	end;
	
	if e.y>0 then begin
		d[3] := c + 2.1;
	end
	else begin
		c := c + 3;
	end;
	
	with e do begin
		y:=1;
	end;
end.