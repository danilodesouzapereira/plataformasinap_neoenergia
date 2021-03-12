    UPDATE
      (SELECT * FROM Cenario) sel
    SET
      sel.strDescricao = [p_Descricao],
      sel.fltTaxa = [p_Taxa]
    WHERE
      sel.intCenarioId = [p_CenarioId];
