    UPDATE
      (SELECT * FROM ConjuntoCurva) sel
    SET
      sel.strDescricao = [p_Descricao],
      sel.bolTipica = [p_Tipica],
      sel.intStatus = [p_Status]
    WHERE
      sel.intConjCurvaId = [p_ConjCurvaId];
