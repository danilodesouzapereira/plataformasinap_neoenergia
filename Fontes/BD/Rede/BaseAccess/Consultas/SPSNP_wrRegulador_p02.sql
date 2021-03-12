    UPDATE
      (SELECT * FROM Regulador) sel
    SET
      sel.intTipoLigacaoId = [p_TipoLigacaoId],
      sel.fltSnom = [p_Snom],
      sel.bolBypass = [p_Bypass],
      sel.fltFaixaVar = [p_FaixaVar],
      sel.intNumPasso = [p_NumPasso],
      sel.bolSentidoReversivel = [p_SentidoReversivel],
      sel.intSentidoBarRefId = IIF(ISNULL( [p_SentidoBarRefId] ), -1, [p_SentidoBarRefId]),
      sel.bolAjusteAuto = [p_AjusteAuto],
      sel.intCfgFixPassoSel = [p_CfgFixPassoSel],
      sel.intCfgAutBarRefId = IIF(ISNULL( [p_CfgAutBarRefId] ), -1, [p_CfgAutBarRefId]),
      sel.fltCfgAutVpu = [p_CfgAutVpu],
      sel.fltZ0R = [p_Z0R],
      sel.fltZ0X = [p_Z0X],
      sel.fltZ1R = [p_Z1R],
      sel.fltZ1X = [p_Z1X],
      sel.intModeloId = [p_ModeloId],
      sel.intTipoFaseId = [p_TipoFaseId],
	  sel.intCfgAutBarRefInvId = IIF(ISNULL( [p_CfgAutBarRefInvId] ), -1, [p_CfgAutBarRefInvId]),
	  sel.fltCfgAutVinvpu = [p_CfgAutVinvpu],
	  sel.fltCfgAutLbmpu = IIF(ISNULL( [p_CfgAutLbmpu] ), 0, [p_CfgAutLbmpu]),
	  sel.fltCfgAutLbmInvpu = IIF(ISNULL( [p_CfgAutLbmInvpu] ), 0, [p_CfgAutLbmInvpu]),
	  sel.intRelTap = [p_RelTap]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intReguladorId = [p_ReguladorId];
