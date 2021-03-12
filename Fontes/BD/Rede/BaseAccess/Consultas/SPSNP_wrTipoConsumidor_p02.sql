    UPDATE
      (SELECT * FROM TipoConsumidor) sel
    SET
      sel.intTag        = [p_Tag],
      sel.strDescricao  = [p_Descricao],
      sel.bolTipico     = [p_Tipico],
      sel.intCorId      = [p_CorId]
    WHERE
      sel.intTipoConsId = [p_TipoConsId];
