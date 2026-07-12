if Boton_Peatones then
	Peticion_Peaton := True;
end_If;

Temp_Minimo_Vehiculos(IN := (Etapa = 1), PT := T#30s);

Temp_Ambar(IN := (Etapa = 2), PT := T#5s);
Temp_Seguridad(IN := (Etapa = 25), PT := T#3s);
Temp_Pea_Verde(IN := (Etapa = 3), PT := T#7s);
Temp_Pea_Verde_Apagado(IN := (Etapa = 4), PT := T#1s);
Temp_Pea_Verde_Encendido(IN := (Etapa = 5), PT := T#1s);


if Etapa = 0 then
	Etapa := 1;
	Contador := 0;
	Peticion_Peaton:= False;
end_if;

Case etapa of
	1:
		Verde := True; Ambar := False; Rojo := False;
        	Verde_Peatones := False; Rojo_Peatones := True;
		
		if Peticion_Peaton and Temp_Minimo_Vehiculos.Q then
			Etapa := 2;
			Peticion_Peaton := False;
		end_if;

	2:
		Verde := False; Ambar := True; Rojo := False;
        	Verde_Peatones := False; Rojo_Peatones := True;

		if Temp_Ambar.Q then
			Etapa := 25;
		end_if;

	25:
		Verde := False; Ambar := False; Rojo := True;
        	Verde_Peatones := False; Rojo_Peatones := True;

		if Temp_Seguridad.Q then
			Etapa := 3;
		end_if;

	3:
		Verde := False; Ambar := False; Rojo := True;
        	Verde_Peatones := True; Rojo_Peatones := False;

		if Temp_Pea_Verde.Q then
			Etapa := 4;
		end_if;

	4:
		Verde := False; Ambar := False; Rojo := True;
        	Verde_Peatones := False; Rojo_Peatones := False;

		if Temp_Pea_Verde_Apagado.Q then
			Etapa := 5;
		end_if;

	5:
		Verde := False; Ambar := False; Rojo := True;
        	Verde_Peatones := True; Rojo_Peatones := False;

		if Temp_Pea_Verde_Encendido.Q then
			Contador := Contador + 1;
			if Contador >= 3 then
				Etapa := 0;
			else
				Etapa := 4;
			end_if;

		end_if;
end_case;
