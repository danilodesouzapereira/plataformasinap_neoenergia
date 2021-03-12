  INSERT INTO
    Barra (intEstudoId, intBarraId, fltVNom, intEsqX1, intEsqY1, intEsqX2, intEsqY2 ,intUtmX, intUtmY)
  SELECT [p_EstudoIdNovo], intBarraId, fltVNom, intEsqX1, intEsqY1, intEsqX2, intEsqY2 ,intUtmX, intUtmY
  FROM Barra WHERE intEstudoId = [p_EstudoIdOrg];

