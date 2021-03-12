    UPDATE
      (SELECT * FROM SuporteFixacao) sel
    SET
      sel.fltX = [p_X],
      sel.fltY = [p_Y],
      sel.strCodigo = [p_Codigo]
    WHERE
      sel.intSuporteId = [p_SuporteId] AND
      sel.intFixacaoItem = [p_FixacaoItem];
