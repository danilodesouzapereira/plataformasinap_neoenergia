    UPDATE
      (SELECT * FROM Barra) sel
    SET
      sel.fltVNom  = [p_VNom],
      sel.intEsqX1 = [p_EsqX1],
      sel.intEsqY1 = [p_EsqY1],
      sel.intEsqX2 = [p_EsqX2],
      sel.intEsqY2 = [p_EsqY2],
      sel.intUtmX  = [p_UtmX],
      sel.intUtmY  = [p_UtmY]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intBarraId = [p_BarraId];
