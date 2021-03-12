    UPDATE
      (SELECT * FROM Trafo3E) sel
    SET
      sel.fltPerdaFerro = [p_PerdaFerro],
      sel.fltSbase = [p_Sbase],
      sel.fltZPS0R = [p_ZPS0R],
      sel.fltZPS0X = [p_ZPS0X],
      sel.fltZPT0R = [p_ZPT0R],
      sel.fltZPT0X = [p_ZPT0X],
      sel.fltZST0R = [p_ZST0R],
      sel.fltZST0X = [p_ZST0X],
      sel.fltZPS1R = [p_ZPS1R],
      sel.fltZPS1X = [p_ZPS1X],
      sel.fltZPT1R = [p_ZPT1R],
      sel.fltZPT1X = [p_ZPT1X],
      sel.fltZST1R = [p_ZST1R],
      sel.fltZST1X = [p_ZST1X]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intTrafo3eId = [p_Trafo3EId];
