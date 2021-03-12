    UPDATE
      (SELECT * FROM Dominio) sel
    SET
      sel.strCodigo  = [p_Codigo],
      sel.dttCriacao = [p_DataCriacao]
    WHERE
      sel.intDominioId = [p_DominioId];
